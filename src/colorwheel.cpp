#include "colorwheel.h"

void olcColorWheel::HSVtoRGB(float h, float s, float v)
{
    // Hue: [0.0, 360.0] |  Saturation: [0.0, 1.0] | Value (Brightness): [0.0, 1.0]
    auto f = [&](float n)
    {
        float k = fmod(n + h / 60.0, 6.0);
        return (v - v * s * fmax(0, fmin(k, fmin(4.0 - k, 1.0))));
    };

    R = (int)(255 * f(5));
    G = (int)(255 * f(3));
    B = (int)(255 * f(1));
}

void olcColorWheel::RGBtoHSV(uint8_t r, uint8_t g, uint8_t b)
{
    // Red: [0, 255] | Blue: [0, 255] | Green: [0, 255]
    V = (float) std::max(r, std::max(g, b)) / (float) 255;
    float C = V - (float) std::min(r, std::min(g, b)) / (float) 255;
    
    if (C == 0.0) { H = 0.0; }
    else if (V == (float) r / (float) 255) { H = fmod(60.0 * (6.0 + (g - b) / (255 * C)), 360.0); }
    else if (V == (float) g / (float) 255) { H = fmod(60.0 * (2.0 + (b - r) / (255 * C)), 360.0); }
    else if (V == (float) b / (float) 255) { H = fmod(60.0 * (4.0 + (r - g) / (255 * C)), 360.0); }

    if (V == 0) { S = 0.0; }
    else { S = C / V; }
}

void olcColorWheel::CalculateColorWheel()
{
    for (int x = -radius; x < radius; x += 1)
    {
        for (int y = -radius; y < radius; y += 1)
        {
            float r = sqrt(x * x + y * y) / (float) radius;
            float angle = atan2(y, x) * 180.0 / M_PI;
            HSVtoRGB(angle, r, V);
            uint8_t alpha = 255;
            if (r > 1.0) 
            { 
                R = 0;
                G = 0;
                B = 0;
                alpha = 0;
            }

            // Fix index in map
            int nMapWidth = 2 * radius;
            int transX = x + radius;
            int transY = y + radius;
            int pixelWidth = 4;
            int index = (transY * nMapWidth + transX) * pixelWidth;
            vPixelMap[index+0] = R;
            vPixelMap[index+1] = G;
            vPixelMap[index+2] = B;
            vPixelMap[index+3] = alpha;
        }
    }
}

void olcColorWheel::DrawColorWheel()
{
    olc::Pixel::Mode currentPixelMode = GetPixelMode();
    SetPixelMode(olc::Pixel::ALPHA);
    for (int x = -radius; x < radius; x += 1)
    {
        for (int y = -radius; y < radius; y += 1)
        {
            int nMapWidth = 2 * radius;
            int transX = x + radius;
            int transY = y + radius;
            int index = (transY * nMapWidth + transX) * pixelWidth;
            olc::Pixel pixel = {vPixelMap[index + 0], 
                                vPixelMap[index + 1],
                                vPixelMap[index + 2],
                                vPixelMap[index + 3]};
            Draw({vCircleOffset.x + x, vCircleOffset.y + y}, pixel);
        }
    }
    SetPixelMode(currentPixelMode);
}

void olcColorWheel::DrawBrightnessBar()
{
    for (int x = 0; x < (radius / 8); x += 1)
    {
        for (int y = -radius; y < radius; y += 1)
        {
            uint8_t brightness = (uint8_t)(255 * ((1 - (float) y / (float) radius) / 2.0));
            Draw({vVBarOffset.x + x, vVBarOffset.y + y}, 
                 {brightness, brightness, brightness});
        }
    }
}

void olcColorWheel::DrawColorChart()
{
    // Draw circle
    DrawColorWheel();
    DrawCircle(vCircleOffset, radius+1, olc::WHITE);
    
    // Draw value bar
    DrawBrightnessBar();
    olc::vi2d BarPos = vVBarOffset + olc::vi2d{0, -radius};
    DrawRect(BarPos, {radius / 8, 2 * radius}, olc::WHITE);
}

bool olcColorWheel::OnUserCreate()
{
    // Calculate pixel map once for a given radius
    vPixelMap = new uint8_t[4 * 2 * radius * 2 * radius];
    CalculateColorWheel();

    return true;
}

bool olcColorWheel::OnUserUpdate(float fElapsedTime)
{
    Clear({50, 50, 50});

    olc::vi2d vMouse = {GetMouseX(), GetMouseY()};

    // Draw color wheel and brightness bar
    DrawColorChart();

    // Handle color selection
    olc::vi2d PosInWheel = vMouse - vCircleOffset;
    int ClickDist = sqrt(PosInWheel.x * PosInWheel.x + 
                         PosInWheel.y * PosInWheel.y);
    if (GetMouse(0).bHeld && (ClickDist < (float) radius))
    {
        int transX = PosInWheel.x + radius;
        int transY = PosInWheel.y + radius;
        int index = (transY * (2 * radius) + transX) * pixelWidth;
        R = vPixelMap[index + 0];
        G = vPixelMap[index + 1];
        B = vPixelMap[index + 2];
        RGBtoHSV(R, G, B);
        vWheelCursor = vMouse;
    }

    // Handle brightness selection
    olc::vi2d PosInBar = vMouse - vVBarOffset - olc::vi2d{radius / 16, 0};
    if (GetMouse(0).bHeld && abs(PosInBar.x) <= (radius / 16) && abs(PosInBar.y) <= radius)
    {
        V = (1 - (float) PosInBar.y / (float) radius) / 2.0;
        CalculateColorWheel();
        HSVtoRGB(H, S, V);
        vBarCursor.y = vMouse.y;
    }

    // Draw selected color cursor 
    DrawCircle(vWheelCursor, 4, olc::WHITE);
    DrawCircle(vWheelCursor, 5, olc::BLACK);

    // Draw selected brightness cursor
    DrawCircle(vBarCursor, 4, olc::WHITE);
    DrawCircle(vBarCursor, 5, olc::BLACK);

    // Draw current HSV
    DrawString({190, 10}, "H:" + std::to_string(H), olc::WHITE, 3);
    DrawString({190, 40}, "S:" + std::to_string(S), olc::WHITE, 3);
    DrawString({190, 70}, "V:" + std::to_string(V), olc::WHITE, 3);

    // Draw current RGBA
    DrawString({10, 10}, "R:" + std::to_string(R), olc::RED, 3);
    DrawString({10, 40}, "G:" + std::to_string(G), olc::GREEN, 3);
    DrawString({10, 70}, "B:" + std::to_string(B), olc::BLUE, 3);

    // Draw current selected color
    FillRect({10, 110}, {50, 50}, {R,G,B});
    DrawRect({9, 109}, {51, 51}, olc::WHITE);

    // Draw mouse position (debug)
    DrawString({10, ScreenHeight() - 50}, "Mouse.x = " + std::to_string(vMouse.x), olc::WHITE, 3);
    DrawString({10, ScreenHeight() - 25}, "Mouse.y = " + std::to_string(vMouse.y), olc::WHITE, 3);

    return true;
}
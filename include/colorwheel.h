#include <cmath>
#include "olcPixelGameEngine.h"

class olcColorWheel : public olc::PixelGameEngine
{
public:
    olcColorWheel() { sAppName = "Color Wheel"; }

private:
    // Color chart settings
    uint8_t pixelWidth = 4;
    uint16_t radius = 200;
    olc::vi2d vCircleOffset = {375, 320}; // Center position (in pixels)
    olc::vi2d vVBarOffset   = vCircleOffset + olc::vi2d{radius + 12, 0};
    olc::vi2d vWheelCursor  = vCircleOffset;
    olc::vi2d vBarCursor    = vVBarOffset + olc::vi2d{radius / 16, -radius};

    // Color variables
    float H = 70.0;     // Hue
    float S = 1.0;      // Saturation
    float V = 1.0;      // Value (Brightness)
    uint8_t R = 255;    // Red
    uint8_t G =   0;    // Green
    uint8_t B =   0;    // Blue
    uint8_t* vPixelMap; // Store RGBA from wheel

    // Drawing functions
    void DrawColorChart();
    void DrawColorWheel();
    void DrawBrightnessBar();
    void CalculateColorWheel();
    void HSVtoRGB(float h, float s, float v);
    void RGBtoHSV(uint8_t r, uint8_t g, uint8_t b);

protected:  
    bool OnUserCreate() override;
    bool OnUserUpdate(float fElapsedTime) override;
};

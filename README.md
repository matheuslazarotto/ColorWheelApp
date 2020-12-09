# Color Wheel application

Implement a demonstration of a color wheel widget chart for color selection. 
The colors are selected in a HSV (Hue, Saturation, Value/Brightness) 
scale and converted to RGB.

The purpose of the code is to provide a raw implementation of both the 
construction of the wheel and the mouse handle commands for selection, 
as a simple tool to learn both C++ and color mapping. The algorithm can 
be easily ported to other graphics engines like SDL or SFML, since the 
drawing part is relatively simple.

The graphic part of the code is based on the well made educational engine 
tool olcPixelGameEngine, which I must credit and thanks Javidx9 for releasing 
it for free. Check out his channel with tons of information on the engine and 
brilliant tutorials:
https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA
and his olcPGE git source:
https://github.com/OneLoneCoder/olcPixelGameEngine

A little bit of color mapping theory, particularly HSV/HSL system and RGB 
can be found in https://en.wikipedia.org/wiki/HSL_and_HSV; which includes 
the conversion formulas used in this application.

This kind of color wheel is commonly found in graphic softwares, particularly 
in Blender and MacOS applications, and I hope it becomes useful in case one 
wish to implemented it by hand in a program and to provide the same amount of 
fun as I had studying it.

Corrections, suggestions or tips are heavily welcomed.
Feel free to enjoy, hack and copy it as your will command.

Implemented by M. Lazarotto (07/12/2020)

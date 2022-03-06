<img src="Screenshots/ModelRenderScreenshot.PNG" alt="Screenshot of render"/>

# HolyGL
A divinely inspired realtime graphics library for the TempleOS continuation
[ZealOS](https://github.com/Zeal-Operating-System/ZealOS/). It aims to stay
true to Terry's objectives by being simple, easy to understand, and
transparent.

If you need features for a project, message me at Professor#8404 on Discord.

# Features in Development
* Multicore rendering.
* New software renderering pipeline for real time games.

# Features Implemented
* Half-Life WAD texture loading.
* BMP texture loading and saving.
* Draw library for 2D primitives.
* Software rasterizer with custom shader support.
    * Currently being rewritten, you can use an older commit if you would
    like to experiment with this (the new renderer will have the same user
    interface).

# Installation
The library can be compiled by including `MakeHolyGL.ZC`, found in the root
of the library. To compile on boot, append `#include "Path/To/MakeHolyGL"`
to the end of `System/MakeSystem.ZC`.

If you use other Holy* libraries, make sure that this library is compiled
before they are, but after HolyMath is.

# Documentation
DolDoc documentation with sprite diagrams is available in `Docs/`.

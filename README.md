# Gr2
A divinely inspired replacement graphics library for ZenithOS. It aims to fullfill Terry's original objectives by being simpler, more stable, and more transparent. 

Rather than hard-programmed switch statements for transformations and color, it uses a signifigantly simplified pipeline of shaders and textures. This does not mean that this library will be a copy of modern graphics API's, instead it will take advantage of running CPU-only to remove obfuscations seen in GPU pipelines.

To start using Gr2, simply include the Gr2.CC from the root of the repository.

	#include "Gr2"

All documentation is available in `Docs/Gr2.DD`
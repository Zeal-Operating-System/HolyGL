
=Gr2=
A divinely inspired replacement graphics library for ZenithOS. It aims to fullfill Terry's original objectives by being simpler, more stable, and more transparent. 

Rather than hard-programmed switch statements for transformations and color, it uses a signifigantly simplified pipeline of shaders and textures. This does not mean that this library will be a copy of modern graphics API's, instead it will take advantage of running CPU-only to remove obfuscations seen in GPU pipelines.

Future Objectives:
*Test/confirm and optimize math library.
*Implement sprites and primitive rendering from the ground up. Rendering primitives with transformations and visual effects can be done using the same shader method, but will not require only triangles to be passed through the pipeline like most graphics APIs. Sprites will allow for custom primitive formats that users can utilize through custom shaders.
*Develop a proper sprite editor. This will require some GUI development as well. 

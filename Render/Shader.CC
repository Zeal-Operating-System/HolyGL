
#define SHD_F64 1
#define SHD_CVEC2 2
#define SHD_CVEC3 3
#define SHD_CVEC4 4
#define SHD_CMat4 5

/*
Render Pipeline:

1) Vertex shader feeds in triangle coordinates in NDC format
   (Normalized Device Coordinates), as well as other
   per-vertex values it would like to interpolate and later 
   feed to the frag shader.

2) Using these NDC triangle coordinates, the rasterizer
   calculates pixel positions of the triangles on the texture,
   and interpolates the per-vertex values given by the vertex
   shader into values for the particular fragment (pixel).

3) The fragment shader is called, with the interpolated
   fragment values fed in, and it calculates the color to
   be drawn at that particular fragment.
*/

class CShader
{
	/* 
	The shader needs an array of I64 indexes to know the
	format of the per-vertex values buffer given to the
	rasterizer by the Vertex Shader.

	For example, if Vec3 Pos, Vec3 Norm, and Vec2 UV were
	to be interpolated per pixel to be given to the fragment shader:

	nVertValues = 3
	VertValues = {SHD_CVEC3, SHD_CVEC3, SHD_CVEC2}
	*/
	I64 nVertValues;
	I64 *vertValues;

	/*
	The vertex shader serves the purpose of returning
	a triangle in NDC coordinates, as well as return a
	buffer containing other per-vertex values.

	Arguments:
		U8 *mdlPtr: 	
			The pointer to the model it needs to
			calculate the triangle coords from.
		U8 *uniforms:
			The pointer to variables unchanged throughout
			entire model render (ie: perspective matrix/lighting/Tex2D)
		I64 iTri:
			Index of current triangle
		I64 nTris:
			Total number of triangles in model
	Returns:
		U8 *CTri
			Current triangle coordinates in NDC
		U8 *vertBuf
			Vertex values to be interpolated for the frag shader.
			These are in format: P0 Values, P1 Values, P2 Values
			Where values has the format defined by VertValues 
	*/  
	U0 (*VertShd)(CTri *tri, F64 *vertOutBuf, U8 *mdlPtr, U8 *uniforms, I64 iTri, I64 nTris);

	/*
	The fragment shader serves the purpose of calculating
	the color at a current fragment given vertex values interpolated
	for that pixel, as well as anything else given in the uniform buffer

	Arguments:
		U8 *vertBuf:
			Interpolated per-vertex values for that frag, eg pos, uv, normal, etc.
		U8 *uniforms
			unchanging variables throughout the render process 
			(ie: perspective matrix/lighting/Tex2D)
	Returns:
		CBG24 *color:
			Color at that particular fragment
	*/
	U0 (*FragShd)(CBGR24 *color, F64 *fragInBuf, U8 *uniforms);
};
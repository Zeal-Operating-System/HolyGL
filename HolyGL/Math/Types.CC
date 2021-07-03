/**
	@defgroup gfxmath Graphics Math
	@brief SSE accelerated graphics math.
*/

/**
	@ingroup gfxmath
	@brief Integer 2D bounding box.
*/
class CBBox2I
{
	I32 x0, y0, x1, y1;
}

/**
	@ingroup gfxmath
	@brief Single-precision floating point 2D vector.

	Must be kept on 16-byte alignment in memory.
*/
class CVec2
{
	I32 x, y, z, w;
};

/**
	@ingroup gfxmath
	@brief Single-precision floating point 3D vector.

	Must be kept on 16-byte alignment in memory.
*/
class CVec3
{
	I32 x, y, z, w;
};

/**
	@ingroup gfxmath
	@brief Single-precision floating point 4D vector.

	Must be kept on 16-byte alignment in memory.
*/
class CVec4
{
	I32 x, y, z, w;
};

/**
	@ingroup gfxmath
	@brief Single-precision floating point 4x4 matrix.

	Must be kept on 16 byte alignment in memory.
*/
class CMat4
{
	U32 e[16];
};

#define SSE_SIZE	4
#define AVX_SIZE	8
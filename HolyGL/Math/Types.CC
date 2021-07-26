/**
	@defgroup Math Matrix Math
	@brief SSE accelerated matrix math.
*/

/**
	@ingroup Math
	@brief Integer 2D bounding box.
*/
class CBBox2I
{
	I32 x0, y0, x1, y1;
};

/**
	@ingroup Math
	@brief Single-precision float.
*/
I32 class F32 {};

/**
	@ingroup Math
	@brief Single-precision floating point 2D vector.

	Must be kept on 16-byte alignment in memory.
*/
class CVec2
{
	I32 x, y, z, w;
};

/**
	@ingroup Math
	@brief Single-precision floating point 3D vector.

	Must be kept on 16-byte alignment in memory.
*/
class CVec3
{
	I32 x, y, z, w;
};

/**
	@ingroup Math
	@brief Single-precision floating point 4D vector.

	Must be kept on 16-byte alignment in memory.
*/
class CVec4
{
	I32 x, y, z, w;
};

/**
	@ingroup Math
	@brief Single-precision floating point 4x4 matrix.

	Stored in column-major. Must be kept on 16 byte alignment in memory.
*/
union CMat4
{
	I32 e[16];
	CVec4 vec[4];
};

// Commonly used constants

#define F32_NEGATE_MASK	0x80000000	// Xor (^) with F32 to negate
#define F32_ZERO		0x00000000
#define F32_ONE			0x3F800000
#define F32_PI			0x40490FDB
#define F32_PI_OVER_180	0x3C8EFA35
#define F32_180_OVER_PI	0x42652EE1
#define F32_PI_OVER_2	0x3FC90FDB	// Pi / 2
#define F32_2_OVER_PI	0x3F22F983	// 2 / Pi

// Indexes into CMat4 element array (CMat4.e[16]).
#define MAT4_00			0
#define MAT4_01			4
#define MAT4_02			8
#define MAT4_03			12
#define MAT4_10			1
#define MAT4_11			5
#define MAT4_12			9
#define MAT4_13			13
#define MAT4_20			2
#define MAT4_21 		6
#define MAT4_22 		10
#define MAT4_23 		14
#define MAT4_30			3
#define MAT4_31			7
#define MAT4_32			11
#define MAT4_33			15

#define SSE_SIZE		4
#define AVX_SIZE		8

#define NDARRAY_I32		0
#define NDARRAY_I64		1
#define NDARRAY_F32		2
#define NDARRAY_F64		3

/**
	@ingroup Math
	@brief N-Dimensional array.

	Array stored in column-major on 16 byte alignment in memory.
*/
class CNDArray
{
	U16 dims;		// Dimensions
	U16 type;		// See NDARRAY_ macros
	U64 *dimLens;	// Dimension lengths
	U64 size;		// Total elements
	I32 *I32Buf;
	I64 *I64Buf;
	F32 *F32Buf;
	F64 *F64Buf;
};

CVec4 *gVec4Temp0 = MAllocAligned(sizeof(CVec4), 16);
CVec4 *gVec4Temp1 = MAllocAligned(sizeof(CVec4), 16);
CVec4 *gVec4Temp2 = MAllocAligned(sizeof(CVec4), 16);
CVec4 *gVec4Temp3 = MAllocAligned(sizeof(CVec4), 16);

CMat4 *gMat4Temp1 = MAllocAligned(sizeof(CMat4), 16);
CMat4 *gMat4Temp2 = MAllocAligned(sizeof(CMat4), 16);

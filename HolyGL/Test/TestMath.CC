Cd(__DIR__);;

#define ARCH_SUPPORTS_AVX	TRUE
#define ARCH_SUPPORTS_AVX2	TRUE

#include "../Math/MakeMath"

I32 s;
CVec4 *a = MAllocAligned(sizeof(CVec4), 16);
CVec4 *b = MAllocAligned(sizeof(CVec4), 16);
CVec4 *dest = MAllocAligned(sizeof(CVec4), 16);
I64 destS;
CVec4 *trueRes = MAllocAligned(sizeof(CVec4), 16);
I32 trueResS;

// Note that some smaller vector functions are identical to larger vector
// functions so they do not get tested. 

//-----------------------------------------------------------------------------
// Vec4Copy

Vec4(1.25, 2.5, 3.75, 5.0, a);
Vec4(0.0, 0.0, 0.0, 0.0, dest);
Vec4Copy(a, dest);
if (!Vec4IsEqual(a, dest))
{
	"[WARNING] Vec4Copy NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}
//-----------------------------------------------------------------------------
// Vec4Add

Vec4(1.25, -2.5, 3.75, -5.0, a);
Vec4(-1.0, 2.0, -3.0, 4.0, b);
Vec4(0.25, -0.5, 0.75, -1.0, trueRes);
Vec4Add(a, b, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4Add NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}

//-----------------------------------------------------------------------------
// Vec4AddS

Vec4(1.25, -2.5, 3.75, -5.0, a);
s = Double2Single(2.0);
Vec4(3.25, -0.5, 5.75, -3.0, trueRes);
Vec4AddS(a, s, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4AddS NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}
//-----------------------------------------------------------------------------
// Vec4Sub

Vec4(1.25, -2.5, 3.75, -5.0, a);
Vec4(-1.0, 2.0, -3.0, 4.0, b);
Vec4(2.25, -4.5, 6.75, -9.0, trueRes);
Vec4Sub(a, b, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4Sub NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}

//-----------------------------------------------------------------------------
// Vec4SubS

Vec4(1.25, -2.5, 3.75, -5.0, a);
s = Double2Single(2.0);
Vec4(-0.75, -4.5, 1.75, -7.0, trueRes);
Vec4SubS(a, s, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4SubS NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}
//-----------------------------------------------------------------------------
// Vec4Mul

Vec4(1.25, -2.5, 3.75, -5.0, a);
Vec4(-1.0, 2.0, -3.0, 4.0, b);
Vec4(-1.25, -5.0, -11.25, -20.0, trueRes);
Vec4Mul(a, b, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4Mul NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}

//-----------------------------------------------------------------------------
// Vec4MulS

Vec4(1.25, -2.5, 3.75, -5.0, a);
s = Double2Single(2.0);
Vec4(2.5, -5.0, 7.5, -10.0, trueRes);
Vec4MulS(a, s, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4MulS NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}
//-----------------------------------------------------------------------------
// Vec4Div

Vec4(1.25, -2.5, 3.75, -5.0, a);
Vec4(-1.0, 2.0, -3.0, 4.0, b);
Vec4(-1.25, -1.25, -1.25, -1.25, trueRes);
Vec4Div(a, b, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4Div NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}

//-----------------------------------------------------------------------------
// Vec4DivS

Vec4(1.25, -2.5, 3.75, -5.0, a);
s = Double2Single(2.0);
Vec4(0.625, -1.25, 1.875, -2.5, trueRes);
Vec4DivS(a, s, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4DivS NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}
//-----------------------------------------------------------------------------
// Vec4Max

Vec4(1.25, -2.5, 3.75, -5.0, a);
Vec4(-1.0, 2.0, -3.0, 4.0, b);
Vec4(1.25, 2.0, 3.75, 4.0, trueRes);
Vec4Max(a, b, dest);
if (!Vec4IsEqual(dest, trueRes))
{
	"[WARNING] Vec4Max NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec3Print(dest);
}
//-----------------------------------------------------------------------------
//

//-----------------------------------------------------------------------------

Free(a);
Free(b);
Free(dest);


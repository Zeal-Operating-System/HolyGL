Cd(__DIR__);;

#define ARCH_SUPPORTS_AVX	FALSE
#define ARCH_SUPPORTS_AVX2	TRUE

#include "../Math/MakeMath"

F32 s;
CVec4 *v0 = MAllocAligned(sizeof(CVec4), 16);
CVec4 *v1 = MAllocAligned(sizeof(CVec4), 16);
CVec4 *v2 = MAllocAligned(sizeof(CVec4), 16);
CMat4 *m1 = MAllocAligned(sizeof(CMat4), 16);
CMat4 *m2 = MAllocAligned(sizeof(CMat4), 16);

CMat4 *destMat = MAllocAligned(sizeof(CMat4), 16);
CVec4 *destVec = MAllocAligned(sizeof(CVec4), 16);

CMat4 *trueMatRes = MAllocAligned(sizeof(CMat4), 16);
CVec4 *trueVecRes = MAllocAligned(sizeof(CVec4), 16);

F32 theta;

CVec4 *countVec = MAllocAligned(sizeof(CVec4), 16);
Vec4(1.0, 2.0, 3.0, 4.0, countVec);

CMat4 *countMat = MAllocAligned(sizeof(CMat4), 16);
Vec4(1.0, 2.0, 3.0, 4.0, &countMat->vec[0]);
Vec4(5.0, 6.0, 7.0, 8.0, &countMat->vec[1]);
Vec4(9.0, 10.0, 11.0, 12.0, &countMat->vec[2]);
Vec4(13.0, 14.0, 15.0, 16.0, &countMat->vec[3]);

//-----------------------------------------------------------------------------
// Mat4MulVec4

Vec4(90.0, 100.0, 110.0, 120.0, trueVecRes);
Mat4MulVec4(countMat, countVec, destVec);
if (!Vec4IsEqual(destVec, trueVecRes))
{
	"[WARNING] Mat4MulVec4 NOT WORKING CORRECTLY! RESULT VECTOR:\n";
	Vec4Print(destVec);
}
//-----------------------------------------------------------------------------
// Mat4Mul

Vec4(90.0,	100.0,	110.0,	120.0,	&trueMatRes->vec[0]);
Vec4(202.0,	228.0,	254.0,	280.0,	&trueMatRes->vec[1]);
Vec4(314.0, 356.0,	398.0,	440.0,	&trueMatRes->vec[2]);
Vec4(426.0,	484.0,	542.0,	600.0,	&trueMatRes->vec[3]);
Mat4Mul(countMat, countMat, destMat);
if (!Mat4IsEqual(destMat, trueMatRes))
{
	"[WARNING] Mat4Mul NOT WORKING CORRECTLY! RESULT MATRIX:\n";
	Mat4Print(destMat);
}
//-----------------------------------------------------------------------------
// Mat4Transpose

Vec4(1.0,	5.0,	9.0,	13.0,	&trueMatRes->vec[0]);
Vec4(2.0,	6.0,	10.0,	14.0,	&trueMatRes->vec[1]);
Vec4(3.0,	7.0,	11.0,	15.0,	&trueMatRes->vec[2]);
Vec4(4.0,	8.0,	12.0,	16.0,	&trueMatRes->vec[3]);
Mat4Transpose(countMat, destMat);
if (!Mat4IsEqual(destMat, trueMatRes))
{
	"[WARNING] Mat4SetTranspose NOT WORKING CORRECTLY! RESULT MATRIX:\n";
	Mat4Print(destMat); 
}
//-----------------------------------------------------------------------------
// Mat4SetTranslation

Vec4(1.0,	0.0,	0.0,	0.0,	&trueMatRes->vec[0]);
Vec4(0.0,	1.0,	0.0,	0.0,	&trueMatRes->vec[1]);
Vec4(0.0,	0.0,	1.0,	0.0,	&trueMatRes->vec[2]);
Vec4(2.0,	3.0,	4.0,	1.0,	&trueMatRes->vec[3]);
Mat4SetTranslation(F642F32(2.0), F642F32(3.0), F642F32(4.0), destMat);
if (!Mat4IsEqual(destMat, trueMatRes))
{
	"[WARNING] Mat4SetTranslation NOT WORKING CORRECTLY! RESULT MATRIX:\n";
	Mat4Print(destMat); 
}
//-----------------------------------------------------------------------------
// Mat4SetScale

Vec4(2.0,	0.0,	0.0,	0.0,	&trueMatRes->vec[0]);
Vec4(0.0,	3.0,	0.0,	0.0,	&trueMatRes->vec[1]);
Vec4(0.0,	0.0,	4.0,	0.0,	&trueMatRes->vec[2]);
Vec4(0.0,	0.0,	0.0,	1.0,	&trueMatRes->vec[3]);
Mat4SetScale(F642F32(2.0), F642F32(3.0), F642F32(4.0), destMat);
if (!Mat4IsEqual(destMat, trueMatRes))
{
	"[WARNING] Mat4SetScale NOT WORKING CORRECTLY! RESULT MATRIX:\n";
	Mat4Print(destMat); 
}
//-----------------------------------------------------------------------------
// Mat4SetRotation

//-----------------------------------------------------------------------------
// Mat4SetLookAt

Vec3(2.0, 3.0, 4.0, v0);	// Eye
Vec3(1.0, -2.0, -3.0, v1);	// Center
Vec3(0.0, 0.0, 1.0, v2);	// Up
Mat4SetLookAt(v0, v1, v2, destMat);
Mat4Print(destMat);
//-----------------------------------------------------------------------------
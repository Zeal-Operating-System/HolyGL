Cd(__DIR__);;

#define ARCH_SUPPORTS_AVX	FALSE
#define ARCH_SUPPORTS_AVX2	TRUE

#include "../Math/MakeMath"

I32 s;
CMat4 *m1 = CAllocAligned(sizeof(CMat4), 16);
CMat4 *m2 = MAllocAligned(sizeof(CMat4), 16);
CMat4 *destMat = MAllocAligned(sizeof(CMat4), 16);
CVec4 *destVec = MAllocAligned(sizeof(CVec4), 16);
I64 destS;
CMat4 *trueMatRes = MAllocAligned(sizeof(CMat4), 16);
CVec4 *trueVecRes = MAllocAligned(sizeof(CVec4), 16);
I32 trueSRes;

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
//-----------------------------------------------------------------------------
// Mat4Projection
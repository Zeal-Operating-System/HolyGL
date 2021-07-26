
F64 F64_QUADRANT_CONSTANT_ARRAY[2] = {0, ã};

#define F64_1	0x3FF0000000000000
#define F64_1_OVER_2_FACTORIAL	0x3FE0000000000000	// 1/2!
#define F64_1_OVER_4_FACTORIAL	0x3FA5555555555555	// 1/4!
#define F64_1_OVER_6_FACTORIAL	0x3F56C16C16C16C17	// 1/6!
#define F64_1_OVER_8_FACTORIAL	0x3EFA01A01A01A01A	// 1/8!
#define F64_1_OVER_10_FACTORIAL	0x3E927E4FB7789F5C	// 1/10!
#define F64_1_OVER_12_FACTORIAL	0x3E21EED8EFF8D898	// 1/12!
#define F64_1_OVER_14_FACTORIAL 0x3DA93974A8C07C9D	// 1/14!

#define F64_2_OVER_PI	0x3FE45F306DC9C883
#define F64_PI_OVER_2	0x3FF921FB54442D18

asm
{
_F32_COS::
	PUSH		RBP
	MOV			RBP, RSP
	PUSH		RBX

//	CVTSS2SD	XMM0, SF_ARG1[RBP]		XMM0: é
	DU8			0xF3, 0x0F, 0x5A, 0x45, 0x10;

/*
	I64 quadrant = é * (2.0 / ã);
*/

	MOV			RAX, F64_2_OVER_PI
//	MOVQ		XMM1, RAX		//	XMM1: 2.0 / Pi
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xC8;
//	MULSD		XMM1, XMM0		//	XMM1: (2.0 / ã) * é
	DU8			0xF2, 0x0F, 0x59, 0xC8;
//	CVTSD2SI	RBX, XMM1		//	RBX : quadrant = (I64) ((2.0 / ã) * é)
	DU8			0xF2, 0x48, 0x0F, 0x2D, 0xD9;

/*
	é = é - (F32) quadrant * (ã / 2.0);
*/

//	CVTSI2SD	XMM1, RBX		//	XMM1: (F32) quadrant
	DU8			0xF2, 0x48, 0x0F, 0x2A, 0xCB;
	MOV			RAX, F64_PI_OVER_2
//	MOVQ		XMM2, RAX		//	XMM2: ã / 2.0
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	MULSD		XMM2, XMM1		//	XMM2: (ã / 2.0) * (F32) quadrant
	DU8			0xF2, 0x0F, 0x59, 0xD1;
//	SUBSD		XMM0, XMM2		//	XMM0: é = é - ((ã / 2.0) * (F32) quadrant)
	DU8			0xF2, 0x0F, 0x5C, 0xC2;

/*
	quadrant += 1;
	é = gTrigQuadrantConstants[(quadrant >> 1) & 1] - é;
*/

	INC			RBX				//	RBX : quadrant += 1
	SHR			RBX, 1			//	RBX : quadrant >> 1
	AND			RBX, 1			//	RBX : (quadrant >> 1) & 1
	SHL			RBX, 3			//	RBX : ((quadrant >> 1) & 1) * sizeof(F64)
	MOV			RAX, &F64_QUADRANT_CONSTANT_ARRAY
	ADD			RBX, RAX		//	RBX : constants + ((quadrant >> 1) & 1)
//	MOVQ		XMM1, [RBX]		//	XMM1: constants[(quadrant >> 1) & 1]
	DU8			0xF3, 0x0F, 0x7E, 0x0B;
//	SUBSD		XMM1, XMM0		//	XMM1: é = constants[(quadrant >> 1) & 1] - é
	DU8			0xF2, 0x0F, 0x5C, 0xC8;

/*
	é2 = -(é * é);
*/

//	MULSD		XMM1, XMM1		// 	XMM1: é * é
	DU8			0xF2, 0x0F, 0x59, 0xC9;
//	XORPS		XMM0, XMM0		//	XMM0: {0.0, 0.0}
	DU8			0x0F, 0x57, 0xC0;
//	SUBSD		XMM0, XMM1		//	XMM0: é2 = -(é * é)
	DU8			0xF2, 0x0F, 0x5C, 0xC1;

/*
	F64 r = (1/14!) * é2;
*/

	MOV			RAX, F64_1_OVER_14_FACTORIAL
//	MOVQ		XMM1, RAX
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xC8;
//	MULSD		XMM1, XMM0
	DU8			0xF2, 0x0F, 0x59, 0xC8;

/*
	r += (1/n!);
	r *= é2;
*/

	MOV			RAX, F64_1_OVER_12_FACTORIAL
//	MOVQ		XMM2, RAX		//	XMM2: (1/6!)
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += (1/6!)
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	MULSD		XMM1, XMM0		//	XMM1: r *= é
	DU8			0xF2, 0x0F, 0x59, 0xC8;

	MOV			RAX, F64_1_OVER_10_FACTORIAL
//	MOVQ		XMM2, RAX		//	XMM2: (1/4!)
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += (1/4!)
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	MULSD		XMM1, XMM0		//	XMM1: r *= é
	DU8			0xF2, 0x0F, 0x59, 0xC8;

	MOV			RAX, F64_1_OVER_8_FACTORIAL
//	MOVQ		XMM2, RAX		//	XMM2: (1/2!)
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += (1/2!)
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	MULSD		XMM1, XMM0		//	XMM1: r *= é
	DU8			0xF2, 0x0F, 0x59, 0xC8;

	MOV			RAX, F64_1_OVER_6_FACTORIAL
//	MOVQ		XMM2, RAX		//	XMM2: (1/2!)
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += (1/2!)
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	MULSD		XMM1, XMM0		//	XMM1: r *= é
	DU8			0xF2, 0x0F, 0x59, 0xC8;

	MOV			RAX, F64_1_OVER_4_FACTORIAL
//	MOVQ		XMM2, RAX		//	XMM2: (1/2!)
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += (1/2!)
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	MULSD		XMM1, XMM0		//	XMM1: r *= é
	DU8			0xF2, 0x0F, 0x59, 0xC8;

	MOV			RAX, F64_1_OVER_2_FACTORIAL
//	MOVQ		XMM2, RAX		//	XMM2: (1/2!)
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += (1/2!)
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	MULSD		XMM1, XMM0		//	XMM1: r *= é
	DU8			0xF2, 0x0F, 0x59, 0xC8;

/*
	return (F32) (r + 1.0)
*/

	MOV			RAX, F64_1
//	MOVQ		XMM2, RAX		//	XMM2: 1.0
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xD0;
//	ADDSD		XMM1, XMM2		//	XMM1: r += 1.0
	DU8			0xF2, 0x0F, 0x58, 0xCA;
//	CVTSD2SS	XMM0, XMM1		//	XMM0: (F32) r
	DU8			0xF2, 0x0F, 0x5A, 0xC1;
//	MOVQ		RAX, XMM0
	DU8			0x66, 0x48, 0x0F, 0x7E, 0xC0;

	POP			RBX
	POP			RBP
	RET1		8
}
/**
	@ingroup Math
	@brief Calculate single precision cosine.

	Based on the paper:
	Fast Trigonometric Functions using Intel's SSE2 Instructions. 2003.
	L. Nyland, M. Snyder 

	@param[in] theta	Angle in radians.
	@return				sin(theta).
*/
_extern _F32_COS F32 F32Cos(F32 theta);

Cd(__DIR__);;
#include "Conversion"

F32 q0 = F32Cos(F642F32(0.785398));
F32 q1 = F32Cos(F642F32(2.35619));
F32 q2 = F32Cos(F642F32(3.92699));
F32 q3 = F32Cos(F642F32(5.49779));
"Cos(45deg)  F32Cos: %n | Cos: %n\n", F322F64(q0), Cos(0.785398);
"Cos(135deg) F32Cos: %n | Cos: %n\n", F322F64(q1), Cos(2.35619);
"Cos(225deg) F32Cos: %n | Cos: %n\n", F322F64(q2), Cos(3.92699);
"Cos(315deg) F32Cos: %n | Cos: %n\n", F322F64(q3), Cos(5.49779);

CGLTex2D graph;
GLTex2DInit(&graph, GL_TEX2D_RAW, 600, 512);
GLTex2DColorFill(&graph, 0xFFFFFF);
GLBindColorTarget(&graph);

F64 temp = (ã / 2) * 100.0;
GLDrawLine(temp, 0, temp, 512, 0x00FF00);

F64 temp = (2 * ã / 2) * 100.0;
GLDrawLine(temp, 0, temp, 512, 0x00FF00);

F64 temp = (3 * ã / 2) * 100.0;
GLDrawLine(temp, 0, temp, 512, 0x00FF00);

I64 i;
F64 angle;
F64 result;
for (i = 0; i < 600; i++)
{
	angle = i * 0.01;
	
	result = Cos(angle);
	result *= -100.0;
	result += 256;
	GLDrawPixel(i, result, 0xFF0000);

	result = F322F64(F32Cos(F642F32(angle)));
	result *= -100.0;
	result += 256;
	GLDrawPixel(i, result, 0x0000FF);
}

while (CharScan() == 0)
{
	GLTex2DDebugDisp(&graph, 16, 16);
	Sleep(1);
}

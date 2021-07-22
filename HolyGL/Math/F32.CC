
F32 F32COS_OFFSET[2] = {
	F32_ZERO,
	F32_PI
};

#define F32COS_IFACT0	0x3F000000	// 1/2!  : 0.5
#define F32COS_IFACT1	0x3ED55555	// 1/4!  : 0.4166...
#define F32COS_IFACT2	0x3AB60B61	// 1/6!  : 0.001388...

asm
{
_F32_COS::
	PUSH		RBP
	MOV			RBP, RSP
	PUSH		RBX

//	At this point, theta should be in the range -pi / 2 < theta < pi / 2

//	MOVSS		XMM0, SF_ARG1[RBP]	// XMM0 = theta
	DU8			0xF3, 0x0F, 0x10, 0x45, 0x10;
//	MOVSS		XMM2, XMM0			// XMM2 = theta
	DU8			0xF3, 0x0F, 0x10, 0xD0;
	//MOV			EAX, F32_2_OVER_PI	// EAX  = 2 / Pi
//	MOVD		XMM1, EAX			// XMM1 = 2 / Pi
	//DU8			0x66, 0x0F, 0x6E, 0xC8;
//	MULSS		XMM0, XMM1			// XMM0 = quadrant = theta * 2 / Pi
	//DU8			0xF3, 0x0F, 0x59, 0xC1;
//	CVTSS2SI	EBX, XMM0			// EBX  = (I32) quadrant (= 0, 1, 2, or 3)
	//DU8			0xF3, 0x0F, 0x2D, 0xD8;
//	CVTSI2SS	XMM0, EBX			// XMM0 = (F32) quadrant
	//DU8			0xF3, 0x0F, 0x2A, 0xC3;
	//MOV			EAX, F32_PI_OVER_2	// EAX  = Pi / 2
//	MOVD		XMM1, EAX			// XMM1 = Pi / 2
	//DU8			0x66, 0x0F, 0x6E, 0xC8;
//	MULSS		XMM0, XMM1			// XMM0 = quadrant * pi / 2
	//DU8			0xF3, 0x0F, 0x59, 0xC1;
//	SUBSS		XMM2, XMM0			// XMM2 = theta = theta - quadrant * Pi / 2
	//DU8			0x0F, 0x0F, 0x5C, 0xD0;
	//INC			EBX					// EBX  = quadrant = quadrant + 1
	//SHR			EBX, 1				// EBX  = quadrant >> 1
	//AND			EBX, 1				// EBX  = (quadrant >> 1) & 1
	//SHL			EBX, 2				// EBX  = sizeof(F32) * ((quadrant >> 1) & 1)
	//MOV			RAX, &F32COS_OFFSET	// RAX  = &F32COS_OFFSET
	//ADD			RAX, RBX			// RAX  = &F32COS_OFFSET[(quadrant >> 1) & 1]
//	MOVSS		XMM0, [RAX]			// XMM0 = 0 or Pi depending on quadrant
	//DU8			0xF3, 0x0F, 0x10, 0x00;
//	SUBSS		XMM0, XMM2			// XMM0 = theta = F32COS_OFFSET - theta
	//DU8			0xF3, 0x0F, 0x5C, 0xC2;
//	MOVSS		XMM1, XMM0			// XMM1 = theta
	//DU8			0xF3, 0x0F, 0x10, 0xC8;
//	MULSS		XMM0, XMM1			// XMM0 = theta * theta
	//DU8			0xF3, 0x0F, 0x59, 0xC1;
//	XORPS		XMM3, XMM3			// XMM3 = 0
	//DU8			0x0F, 0x57, 0xDB;
//	SUBSS		XMM3, XMM0			// XMM3 = theta2 = -(theta * theta)
	//DU8			0xF3, 0x0F, 0x5C, 0xD8;

//	3 Terms

	//MOV			EAX, F32COS_IFACT2
//	MOVD		XMM0, EAX			// XMM0 = r = F32COS_IFACT2
	//DU8			0x66, 0x0F, 0x6E, 0xC0;
//	MULSS		XMM0, XMM3			// XMM0 = r = F32COS_IFACT * theta2
	//DU8			0xF3, 0x0F, 0x59, 0xC3;

	//MOV			EAX, F32COS_IFACT1
//	MOVD		XMM1, EAX			// XMM1 = F32COS_IFACT1
	//DU8			0x66, 0x0F, 0x6E, 0xC8;
//	ADDSS		XMM0, XMM1			// XMM0 = r += F32COS_IFACT1
	//DU8			0xF3, 0x0F, 0x58, 0xC1;
//	MULSS		XMM0, XMM3			// XMM0 = r *= theta2
	//DU8			0xF3, 0x0F, 0x59, 0xC3;

	//MOV			EAX, F32COS_IFACT0
//	MOVD		XMM1, EAX
	//DU8			0x66, 0x0F, 0x6E, 0xC8;
//	ADDSS		XMM0, XMM1			// XMM0 = r += F32COS_IFACT0
	//DU8			0xF3, 0x0F, 0x58, 0xC1;
//	MULSS		XMM0, XMM3			// XMM0 = r *= theta2
	//DU8			0xF3, 0x0F, 0x59, 0xC3;

//	MOVD		EAX, XMM0
	//DU8			0x66, 0x0F, 0x7E, 0xC0;

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
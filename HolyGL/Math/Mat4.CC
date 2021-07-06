
/**
	@ingroup gfxmath
	@brief Print 4x4 matrix.
	
	@param[in] m	4x4 matrix.
*/
U0 Mat4Print(CMat4 *m)
{	
	U8 reg R14 str = "%n\t%n\t%n\t%n\n\n";
asm
{
		PUSH		R15

		XOR			R15, R15			// = i = 0
//		for (RBX = 0; RBX < 4; RBX++)
@@05:
		MOV			RAX, R15			// = i
		SHL			RAX, 2				// = i * 4
		ADD			RAX, SF_ARG1[RBP]	// = &m + i * 4

		SUB			RSP, 32

//		CVTSS2SD	XMM0, 48[RAX]
		DU8			0xF3, 0x0F, 0x5A, 0x40, 0x30;
//		MOVSD		24[RSP], XMM0
		DU8			0xF2, 0x0F, 0x11, 0x44, 0x24, 0x18;

//		CVTSS2SD	XMM0, 32[RAX]
		DU8			0xF3, 0x0F, 0x5A, 0x40, 0x20;
//		MOVSD		16[RSP], XMM0
		DU8			0xF2, 0x0F, 0x11, 0x44, 0x24, 0x10;

//		CVTSS2SD	XMM0, 16[RAX]
		DU8			0xF3, 0x0F, 0x5A, 0x40, 0x10;
//		MOVSD		8[RSP], XMM0
		DU8			0xF2, 0x0F, 0x11, 0x44, 0x24, 0x08;

//		CVTSS2SD	XMM0, [RAX]
		DU8			0xF3, 0x0f, 0x5A, 0x00;
//		MOVSD		[RSP], XMM0
		DU8			0xF2, 0x0F, 0x11, 0x04, 0x24;

		PUSH		4
		PUSH		R14
		CALL		&Print
		ADD			RSP, 48

		INC			R15
		CMP			R15, 4
		JNE			@@05

		POP			R15
}
}

/**
	@ingroup gfxmath
	@brief Check if two 4x4 matrices are equal.

	@param[in] a	Matrix 1
	@param[in] b	Matrix 2
*/
Bool Mat4IsEqual(CMat4 *a, CMat4 *b)
{
	I64 i, j,
		total = 0;

	for (i = 0; i < 4; i++)
	{
		//total += a.vec
		//Vec4IsEqual(a->vec[i], b->vec[i]);
	}
	
	if (total == 4)
		return TRUE;
	else
		return FALSE;
}

asm
{
_MAT4_MUL_VEC4::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG2[RBP]
#if ARCH_SUPPORTS_AVX
//	VBROADCASTSS	XMM0, [RAX]
	DU8				0xC4, 0xE2, 0x79, 0x18, 0x00;
//	VBROADCASTSS	XMM1, 4[RAX]
	DU8				0xC4, 0xE2, 0x79, 0x18, 0x48, 0x04;
//	VBROADCASTSS	XMM2, 8[RAX]
	DU8				0xC4, 0xE2, 0x79, 0x18, 0x50, 0x08;
//	VBROADCASTSS	XMM3, 12[RAX]
	DU8				0xC4, 0xE2, 0x79, 0x18, 0x58, 0x0C;
#else
//	MOVAPS		XMM0, [RAX]
	DU8			0x0F, 0x28, 0x00;
//	MOVAPS		XMM1, XMM0
	DU8			0x0F, 0x28, 0xC8;
//	MOVAPS		XMM2, XMM0
	DU8			0x0F, 0x28, 0xD0;
//	MOVAPS		XMM3, XMM0
	DU8			0x0F, 0x28, 0xD8;
//	SHUFPS		XMM0, XMM0, 0x00 (0, 0, 0, 0)
	DU8			0x0F, 0xC6, 0xC0, 0x00;
//	SHUFPS		XMM1, XMM1, 0x55 (1, 1, 1, 1)
	DU8			0x0F, 0xC6, 0xC9, 0x55;
//	SHUFPS		XMM2, XMM2, 0xAA (2, 2, 2, 2)
	DU8			0x0F, 0xC6, 0xD2, 0xAA;
//	SHUFPS		XMM3, XMM3, 0xFF (3, 3, 3, 3)
	DU8			0x0F, 0xC6, 0xDB, 0xFF;
#endif

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM4, [RAX]
	DU8			0x0F, 0x28, 0x20;
//	MOVAPS		XMM5, 16[RAX]
	DU8			0x0F, 0x28, 0x68, 0x10;
//	MOVAPS		XMM6, 32[RAX]
	DU8			0x0F, 0x28, 0x70, 0x20;
//	MOVAPS		XMM7, 48[RAX]
	DU8			0x0F, 0x28, 0x78, 0x30;

//	MULPS		XMM4, XMM0
	DU8			0x0F, 0x59, 0xE0;
//	MULPS		XMM5, XMM1
	DU8			0x0F, 0x59, 0xE9;
//	MULPS		XMM6, XMM2
	DU8			0x0F, 0x59, 0xF2;
//	MULPS		XMM7, XMM3
	DU8			0x0F, 0x59, 0xFB;

//	ADDPS		XMM4, XMM5
	DU8			0x0F, 0x58, 0xE5;
//	ADDPS		XMM6, XMM7
	DU8			0x0F, 0x58, 0xF7;
//	ADDPS		XMM4, XMM6
	DU8			0x0F, 0x58, 0xE6;

	MOV			RAX, SF_ARG3[RBP]
//	MOVAPS		[RAX], XMM4
	DU8			0x0F, 0x29, 0x20;
	
	POP			RBP
	RET1		24
}
/**
	@ingroup gfxmath
	@brief Multiply 4x4 matrix by 4D vector.

	@param[in] m		4x4 matrix.
	@param[in] v		4D vector.
	@param[in,out] dest	Destination 4D vector.
*/
_extern _MAT4_MUL_VEC4 U0 Mat4MulVec4(CMat4 *m, CVec4 *v, CVec4 *dest);
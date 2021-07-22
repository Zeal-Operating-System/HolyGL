
/**
	@ingroup Math
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
	@ingroup Math
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
		total += Vec4IsEqual(&a->vec[i], &b->vec[i]);
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

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM4, [RAX]
	DU8			0x0F, 0x28, 0x20;
//	MOVAPS		XMM5, 16[RAX]
	DU8			0x0F, 0x28, 0x68, 0x10;
//	MOVAPS		XMM6, 32[RAX]
	DU8			0x0F, 0x28, 0x70, 0x20;
//	MOVAPS		XMM7, 48[RAX]
	DU8			0x0F, 0x28, 0x78, 0x30;

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
	@ingroup Math
	@brief Multiply 4x4 matrix by 4D vector.

	@param[in] m		4x4 matrix.
	@param[in] v		4D vector.
	@param[in,out] dest	Destination 4D vector.
*/
_extern _MAT4_MUL_VEC4 U0 Mat4MulVec4(CMat4 *m, CVec4 *v, CVec4 *dest);

asm
{
_MAT4_MUL::
	PUSH		RBP
	MOV			RBP, RSP
	PUSH		R14
	PUSH		R15

	MOV			RAX, SF_ARG1[RBP]
	MOV			R14, SF_ARG2[RBP]
	MOV			R15, SF_ARG3[RBP]

//	MOVAPS		XMM4, [RAX]
	DU8			0x0F, 0x28, 0x20;
//	MOVAPS		XMM5, 16[RAX]
	DU8			0x0F, 0x28, 0x68, 0x10;
//	MOVAPS		XMM6, 32[RAX]
	DU8			0x0F, 0x28, 0x70, 0x20;
//	MOVAPS		XMM7, 48[RAX]
	DU8			0x0F, 0x28, 0x78, 0x30;

	MOV			RAX, 4	// = i = 4
@@05:
//	MOVAPS		XMM0, [R14]
	DU8			0x41, 0x0F, 0x28, 0x06;
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

//	MULPS		XMM0, XMM4
	DU8			0x0F, 0x59, 0xC4;
//	MULPS		XMM1, XMM5
	DU8			0x0F, 0x59, 0xCD;
//	MULPS		XMM2, XMM6
	DU8			0x0F, 0x59, 0xD6;
//	MULPS		XMM3, XMM7
	DU8			0x0F, 0x59, 0xDF;

//	ADDPS		XMM0, XMM1
	DU8			0x0F, 0x58, 0xC1;
//	ADDPS		XMM2, XMM3
	DU8			0x0F, 0x58, 0xD3;
//	ADDPS		XMM0, XMM2
	DU8			0x0F, 0x58, 0xC2;

//	MOVAPS		[R15], XMM0
	DU8			0x41, 0x0F, 0x29, 0x07;

	ADD			R14, 16
	ADD			R15, 16

	DEC			RAX
	JNZ			@@05

	POP			R15
	POP			R14
	POP			RBP
	RET1		24
}
/**
	@ingroup Math
	@brief Multiply 4x4 matrix by 4x4 matrix.

	@param[in]     a	4x4 matrix.
	@param[in]     b	4x4 matrix.
	@param[in,out] dest	Destination 4x4 matrix.
*/
_extern _MAT4_MUL U0 Mat4Mul(CMat4 *a, CMat4 *b, CMat4 *dest);

asm
{
_MAT4_TRANSPOSE::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM0, [RAX]
	DU8			0x0F, 0x28, 0x00;
//	MOVAPS		XMM1, XMM0
	DU8			0x0F, 0x28, 0xC8;
//	MOVAPS		XMM2, 16[RAX]
	DU8			0x0F, 0x28, 0x50, 0x10;

//	MOVAPS		XMM3, 32[RAX]
	DU8			0x0F, 0x28, 0x58, 0x20;
//	MOVAPS		XMM4, XMM3
	DU8			0x0F, 0x28, 0xE3;
//	MOVAPS		XMM5, 48[RAX]
	DU8			0x0F, 0x28, 0x68, 0x30;

//	SHUFPS		XMM0, XMM2, 0x44 (0, 1, 0, 1)	// TMP 0
	DU8			0x0F, 0xC6, 0xC2, 0x44;
//	SHUFPS		XMM1, XMM2, 0xEE (2, 3, 2, 3)	// TMP 2
	DU8			0x0F, 0xC6, 0xCA, 0xEE;
//	SHUFPS		XMM3, XMM5, 0x44 (0, 1, 0, 1)	// TMP 1
	DU8			0x0F, 0xC6, 0xDD, 0x44; 
//	SHUFPS		XMM4, XMM5, 0xEE (2, 3, 2, 3)   // TMP 3
	DU8			0x0F, 0xC6, 0xE5, 0xEE;

//	MOVAPS		XMM2, XMM0						// XMM2 = TMP 0
	DU8			0x0F, 0x28, 0xD0;
//	MOVAPS		XMM5, XMM1						// XMM5 = TMP 2
	DU8			0x0F, 0x28, 0xE9;

//	SHUFPS		XMM0, XMM3, 0x88 (0, 2, 0, 2)
	DU8			0x0F, 0xC6, 0xC3, 0x88;
//	SHUFPS		XMM2, XMM3, 0xDD (1, 3, 1, 3)
	DU8			0x0F, 0xC6, 0xD3, 0xDD;
//	SHUFPS		XMM1, XMM4, 0x88 (0, 2, 0, 2)
	DU8			0x0F, 0xC6, 0xCC, 0x88;
//	SHUFPS		XMM5, XMM4, 0xDD (1, 3, 1, 3)
	DU8			0x0F, 0xC6, 0xEC, 0xDD;

	MOV			RAX, SF_ARG2[RBP]
//	MOVAPS		[RAX], XMM0
	DU8			0x0F, 0x29, 0x00;
//	MOVAPS		16[RAX], XMM2
	DU8			0x0F, 0x29, 0x50, 0x10;
//	MOVAPS		32[RAX], XMM1
	DU8			0x0F, 0x29, 0x48, 0x20;
//	MOVAPS		48[RAX], XMM5
	DU8			0x0F, 0x29, 0x68, 0x30;

	POP			RBP
	RET1		16
}
/**
	@ingroup Math
	@brief Transpose 4x4 matrix.

	@param[in]     m	4x4 matrix.
	@param[in,out] dest	Destination 4x4 matrix.
*/
_extern _MAT4_TRANSPOSE U0 Mat4Transpose(CMat4 *m, CMat4 *dest);

/**
	@ingroup Math
	@brief Clear 4x4 matrix and set it to translation transformation.

	@param[in]     x	X translation.
	@param[in]     y	Y translation.
	@param[in]     z	Z translation.
	@param[in,out] dest	Destination 4x4 matrix. 
*/
U0 Mat4SetTranslation(F32 x, F32 y, F32 z, CMat4 *dest)
{
	dest->e[MAT4_00] = F32_ONE;
	dest->e[MAT4_10] = F32_ZERO;
	dest->e[MAT4_20] = F32_ZERO;
	dest->e[MAT4_30] = F32_ZERO;

	dest->e[MAT4_01] = F32_ZERO;
	dest->e[MAT4_11] = F32_ONE;
	dest->e[MAT4_21] = F32_ZERO;
	dest->e[MAT4_31] = F32_ZERO;

	dest->e[MAT4_02] = F32_ZERO;
	dest->e[MAT4_12] = F32_ZERO;
	dest->e[MAT4_22] = F32_ONE;
	dest->e[MAT4_32] = F32_ZERO;

	dest->e[MAT4_03] = x;
	dest->e[MAT4_13] = y;
	dest->e[MAT4_23] = z;
	dest->e[MAT4_33] = F32_ONE;
}

/**
	@ingroup Math
	@brief Clear 4x4 matrix and set it to scale transformation.

	@param[in]     x	X scale.
	@param[in]     y	Y scale.
	@param[in]     z	Z scale.
	@param[in,out] dest	Destination 4x4 matrix.
*/
U0 Mat4SetScale(F32 x, F32 y, F32 z, CMat4 *dest)
{
	dest->e[MAT4_00] = x;
	dest->e[MAT4_10] = F32_ZERO;
	dest->e[MAT4_20] = F32_ZERO;
	dest->e[MAT4_30] = F32_ZERO;

	dest->e[MAT4_01] = F32_ZERO;
	dest->e[MAT4_11] = y;
	dest->e[MAT4_21] = F32_ZERO;
	dest->e[MAT4_31] = F32_ZERO;

	dest->e[MAT4_02] = F32_ZERO;
	dest->e[MAT4_12] = F32_ZERO;
	dest->e[MAT4_22] = z;
	dest->e[MAT4_32] = F32_ZERO;

	dest->e[MAT4_03] = F32_ZERO;
	dest->e[MAT4_13] = F32_ZERO;
	dest->e[MAT4_23] = F32_ZERO;
	dest->e[MAT4_33] = F32_ONE;
}

/**
	@ingroup Math
	@brief Clear 4x4 matrix and set it to a look-at transform.

			 Ú                ¿   Ú            ¿   Ú                    ¿
			 ³Rx   Ry   Rz   0³   ³1  0  0  -Ex³   ³Rx  Ry  Rz  -(R ù E)³ 
	LookAt = ³Ux   Uy   Uz   0³ * ³0  1  0  -Ey³ = ³Ux  Uy  Ut  -(U ù E)³
			 ³-Fx  -Fy  -Fz  0³   ³0  0  1  -Ez³   ³Fx  Fy  Fz  F ù E   ³
			 ³0    0    0    1³   ³0  0  0  1  ³   ³0   0   0   1       ³
             À                Ù   À            Ù   À                    Ù
	Where:
	ùR is the eye right direction.
	ùU is the eye up direction.
	ùF is the eye forward direction (away from target towards viewer).
	ùE is the position of the eye.

	@param[in]     eye		Position of eye.
	@param[in]     center	Position to look at.
	@param[in]     up		Up direction vector. Can be general like an axis,
							does not need to be orthographic in relation
							to camera angle.
	@param[in,out] dest		Destination 4x4 matrix.
*/
U0 Mat4SetLookAt(CVec3 *eye, CVec3 *center, CVec3 *up, CMat4 *dest)
{
	Vec3Sub(center, eye, gVec4Temp0);				// Forward vector
	Vec3Normalize(gVec4Temp0, gVec4Temp0);

	Vec3Cross(gVec4Temp0, up, gVec4Temp1);			// Right vector
	Vec3Normalize(gVec4Temp1, gVec4Temp1);

	Vec3Cross(gVec4Temp1, gVec4Temp0, gVec4Temp2);	// Correct Up vector

	// Use dot products to apply eye translation matrix without a 4x4 multiply.
	dest->e[MAT4_03] = Vec3Dot(gVec4Temp1, eye) ^ F32_NEGATE_MASK;
	dest->e[MAT4_13] = Vec3Dot(gVec4Temp2, eye) ^ F32_NEGATE_MASK;
	dest->e[MAT4_23] = Vec3Dot(gVec4Temp0, eye);
	dest->e[MAT4_33] = F32_ONE;

	Vec3Negate(gVec4Temp0, gVec4Temp0);				// Forward = -Forward

	dest->e[MAT4_00] = gVec4Temp1->x;				// Right
	dest->e[MAT4_10] = gVec4Temp2->x;				// Up
	dest->e[MAT4_20] = gVec4Temp0->x;				// Forward
	dest->e[MAT4_30] = F32_ZERO;

	dest->e[MAT4_01] = gVec4Temp1->y;
	dest->e[MAT4_11] = gVec4Temp2->y;
	dest->e[MAT4_21] = gVec4Temp0->y;
	dest->e[MAT4_31] = F32_ZERO;

	dest->e[MAT4_02] = gVec4Temp1->z;
	dest->e[MAT4_12] = gVec4Temp2->z;
	dest->e[MAT4_22] = gVec4Temp0->z;
	dest->e[MAT4_32] = F32_ZERO;
}







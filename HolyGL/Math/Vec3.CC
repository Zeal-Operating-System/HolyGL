
asm
{
_VEC3::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG4[RBP]	// = *dest

//	CVTSD2SS	XMM0, SF_ARG1[RBP]
	DU8			0xF2, 0x0F, 0x5A, 0x45, 0x10;
//	MOVSS		[RAX], XMM0
	DU8			0xF3, 0x0F, 0x11, 0x00;

//	CVTSD2SS	XMM0, SF_ARG1[RBP]
	DU8			0xF2, 0x0F, 0x5A, 0x45, 0x18;
//	MOVSS		4[RAX], XMM0
	DU8			0xF3, 0x0F, 0x11, 0x40, 0x04;

//	CVTSD2SS	XMM0, SF_ARG1[RBP]
	DU8			0xF2, 0x0F, 0x5A, 0x45, 0x20;
//	MOVSS		8[RAX], XMM0
	DU8			0xF3, 0x0F, 0x11, 0x40, 0x08;

	POP			RBP
	RET1		32
}
/**
	@ingroup Math
	@brief Initialize members of a vector with double-precision floats.

	@param[in]     x	X component.
	@param[in]     y	Y component.
	@param[in]     z	Z component.
	@param[in,out] dest	Destination
*/
_extern _VEC3 U0 Vec3(F64 x, F64 y, F64 z, CVec3 *dest);

/**
	@ingroup Math
	@brief Print members of a vector.

	@param[in] v	Vector
*/
U0 Vec3Print(CVec3 *v)
{
	U8 reg R15 str = "%n\t%n\t%n\n\n";
asm
{
	MOV			RAX, SF_ARG1[RBP]	// = *v
	SUB			RSP, 24

//	CVTSS2SD	XMM0, 8[RAX]
	DU8			0xF3, 0x0F, 0x5A, 0x40, 0x08;
//	MOVSD		16[RSP], XMM0
	DU8			0xF2, 0x0F, 0x11, 0x44, 0x24, 0x10;

//	CVTSS2SD	XMM0, 4[RAX]
	DU8			0xF3, 0x0F, 0x5A, 0x40, 0x04;
//	MOVSD		8[RSP], XMM0
	DU8			0xF2, 0x0F, 0x11, 0x44, 0x24, 0x08;

//	CVTSS2SD	XMM0, [RAX]
	DU8			0xF3, 0x0f, 0x5A, 0x00;
//	MOVSD		[RSP], XMM0
	DU8			0xF2, 0x0F, 0x11, 0x04, 0x24;

	PUSH		3					// # of varargs
	PUSH		R15
	CALL		&Print
	ADD			RSP, 40
}
}

/**
	@ingroup Math
	@brief Copy all members of a vector to destination.

	@param[in]     src	Source
	@param[in,out] dest	Destination 
*/
_extern _VEC4_COPY U0 Vec3Copy(CVec3 *src, CVec3 *dest);

asm
{
_VEC3_IS_EQUAL::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG1[RBP]	// = *a
//	MOVAPS		XMM0, [RAX]
	DU8			0x0F, 0x28, 0x00;
	MOV			RAX, SF_ARG2[RBP]	// = *b
//	MOVAPS		XMM1, [RAX]
	DU8			0x0F, 0x28, 0x08;
//	CMPPS		XMM0, XMM1, 0		// CMPEQPS
	DU8			0x0F, 0xC2, 0xC1, 0x00;

//	PMOVMSKB	RAX, XMM0
	DU8			0x66, 0x0F, 0xD7, 0xC0;
	AND			RAX, 0xFFF
	CMP			RAX, 0xFFF
	JNZ			_is_not_equal
	MOV			RAX, 1
	JMP			_return
_is_not_equal:
	MOV			RAX, 0

_return:
	POP		RBP
	RET1	16
}
/**
	@ingroup Math
	@brief Checks if two vectors are equal.

	@param[in] a	Vector 1
	@param[in] b	Vector 2
*/
_extern _VEC3_IS_EQUAL Bool Vec3IsEqual(CVec3 *a, CVec3 *b);

/**
	@ingroup Math
	@brief Sum of two vectors.

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC4_ADD U0 Vec3Add(CVec3 *a, CVec3 *b, CVec3 *dest);

/**
	@ingroup Math
	@brief Add a scalar to a vector.

	@param[in]     v	Vector
	@param[in]     s	Scalar
	@param[in,out] dest	Destination
*/
_extern _VEC4_ADDS U0 Vec3AddS(CVec3 *v, I32 *s, CVec3 *dest);

/**
	@ingroup Math
	@brief Difference of two vectors.

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC4_SUB U0 Vec3Sub(CVec3 *a, CVec3 *b, CVec3 *dest);

/**
	@ingroup Math
	@brief Subtract a scalar from a vector.

	@param[in]     v	Vector
	@param[in]     s	Scalar
	@param[in,out] dest	Destination
*/
_extern _VEC4_SUBS U0 Vec3SubS(CVec3 *v, I32 *s, CVec3 *dest);

/**
	@ingroup Math
	@brief Product  of two vectors (element multiplication).

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC4_MUL U0 Vec3Mul(CVec3 *a, CVec3 *b, CVec3 *dest);

/**
	@ingroup Math
	@brief Scale a vector by a scalar.

	@param[in]     v	Vector
	@param[in]     s	Scalar
	@param[in,out] dest	Destination
*/
_extern _VEC4_MULS U0 Vec3MulS(CVec3 *v, I32 *s, CVec3 *dest);

/**
	@ingroup Math
	@brief Quotient of two vectors.

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC4_DIV U0 Vec3Div(CVec3 *a, CVec3 *b, CVec3 *dest);

/**
	@ingroup Math
	@brief Divide a vector by a scalar.

	@param[in]     v	Vector
	@param[in]     s	Scalar
	@param[in,out] dest	Destination
*/
_extern _VEC4_DIVS U0 Vec3DivS(CVec3 *v, I32 *s, CVec3 *dest);

/**
	@ingroup Math
	@brief Min of two vectors (element-wise).

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC4_MIN U0 Vec3Min(CVec3 *a, CVec3 *b, CVec3 *dest);

/**
	@ingroup Math
	@brief Max of two vectors (element-wise).

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC4_MAX U0 Vec3Max(CVec3 *a, CVec3 *b, CVec3 *dest);

/**
	@ingroup Math
	@brief Negate a vector (elements = -elements).

	@param[in]     v	Vector
	@param[in,out] dest	Destination
*/
_extern _VEC4_NEGATE U0 Vec3Negate(CVec3 *v, CVec3 *dest);

asm
{
_VEC3_NORMALIZE::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM0, [RAX]
	DU8			0x0F, 0x28, 0x00;
//	MOVAPS		XMM2, XMM0
	DU8			0x0F, 0x28, 0xD0;
//	MULPS		XMM0, XMM2
	DU8			0x0F, 0x59, 0xC2;

//	MOVHLPS		XMM1, XMM0
	DU8			0x0F, 0x12, 0xC8;
//	ADDSS		XMM1, XMM0
	DU8			0xF3, 0x0F, 0x58, 0xC8;
//	SHUFPS		XMM0, XMM0, 0x55 (1, 1, 1, 1)
	DU8			0x0F, 0xC6, 0xC0, 0x55;
//	ADDSS		XMM0, XMM1
	DU8			0xF3, 0x0F, 0x58, 0xC1;
//	SQRTSS		XMM0, XMM0
	DU8			0xF3, 0x0F, 0x51, 0xC0;
//	SHUFPS		XMM0, XMM0, 0x00 (0, 0, 0, 0)
	DU8			0x0F, 0xC6, 0xC0, 0x00;
//	DIVPS		XMM2, XMM0
	DU8			0x0F, 0x5E, 0xD0;

	MOV			RAX, SF_ARG2[RBP]
//	MOVAPS		[RAX], XMM2
	DU8			0x0F, 0x29, 0x10;

	POP			RBP
	RET1		16
}
/**
	@ingroup Math
	@brief Normalize a vector (length = 1.0).

	@param[in]     v	Vector
	@param[in,out] dest	Destination
*/
_extern _VEC3_NORMALIZE U0 Vec3Normalize(CVec3 *v, CVec3 *dest);

asm
{
_VEC3_DOT::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM0, [RAX]
	DU8			0x0F, 0x28, 0x00;
	MOV			RAX, SF_ARG2[RBP]
//	MOVAPS		XMM1, [RAX]
	DU8			0x0F, 0x28, 0x08;
//	MULPS		XMM0, XMM1
	DU8			0x0F, 0x59, 0xC1;

//	MOVHLPS		XMM1, XMM0
	DU8			0x0F, 0x12, 0xC8;
//	ADDSS		XMM1, XMM0
	DU8			0xF3, 0x0F, 0x58, 0xC8;
//	SHUFPS		XMM0, XMM0, 0x55 (1, 1, 1, 1)
	DU8			0x0F, 0xC6, 0xC0, 0x55;
//	ADDSS		XMM0, XMM1
	DU8			0xF3, 0x0F, 0x58, 0xC1;
//	MOVQ		RAX, XMM0
	DU8			0x66, 0x48, 0x0F, 0x7E, 0xC0;

	POP			RBP
	RET1		16
}
/**
	@ingroup Math
	@brief Dot product of two vectors.

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@return				Dot product.
*/
_extern _VEC3_DOT I32 Vec3Dot(CVec3 *a, CVec3 *b);

asm
{
_VEC3_CROSS::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM0, [RAX]
	DU8			0x0F, 0x28, 0x00;
//	MOVAPS		XMM1, XMM0
	DU8			0x0F, 0x28, 0xC8;
	MOV			RAX, SF_ARG2[RBP]
//	MOVAPS		XMM2, [RAX]
	DU8			0x0F, 0x28, 0x10;
//	MOVAPS		XMM3, XMM2
	DU8			0x0F, 0x28, 0xDA;

//	SHUFPS		XMM0, XMM0, 0xC9 (1, 2, 0, 3) [Ay Az Ax]
	DU8			0x0F, 0xC6, 0xC0, 0xC9;
//	SHUFPS		XMM1, XMM1, 0xD2 (2, 0, 1, 3) [Az Ax Ay]
	DU8			0x0F, 0xC6, 0xC9, 0xD2;
//	SHUFPS		XMM2, XMM2, 0xD2 (2, 0, 1, 3) [Bz Bx By]
	DU8			0x0F, 0xC6, 0xD2, 0xD2;
//	SHUFPS		XMM3, XMM3, 0xC9 (1, 2, 0, 3) [By Bz Bx]
	DU8			0x0F, 0xC6, 0xDB, 0xC9;

//	MULPS		XMM0, XMM2
	DU8			0x0F, 0x59, 0xC2;
//	MULPS		XMM1, XMM3
	DU8			0x0F, 0x59, 0xCB;
//	SUBPS		XMM0, XMM1
	DU8			0x0F, 0x5C, 0xC1;

	MOV			RAX, SF_ARG3[RBP]
//	MOVAPS		[RAX], XMM0
	DU8			0x0F, 0x29, 0x00;

	POP			RBP
	RET1		24
}
/**
	@ingroup Math
	@brief Cross product of two vectors.

	@param[in]     a	Vector 1
	@param[in]     b	Vector 2
	@param[in,out] dest	Destination
*/
_extern _VEC3_CROSS U0 Vec3Cross(CVec3 *a, CVec3 *b, CVec3 *dest);













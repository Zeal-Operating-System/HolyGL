asm
{
_F64_2_F32::
	PUSH		RBP
	MOV			RBP, RSP

//	CVTSD2SS	XMM0, SF_ARG1[RBP]
	DU8			0xF2, 0x0F, 0x5A, 0x45, 0x10;
//	MOVQ		RAX, XMM0
	DU8			0x66, 0x48, 0x0F, 0x7E, 0xC0;

	POP			RBP
	RET1		8
}
/**
	@ingroup Math
	@brief Convert double-precision scalar to single-precision.

	@param[in] d	Double-precision scalar.
	@return			Single-precision scalar.
*/
_extern _F64_2_F32 F32 F642F32(F64 d);

asm
{
_F32_2_F64::
	PUSH		RBP
	MOV			RBP, RSP

//	CVTSS2SD	XMM0, SF_ARG1[RBP]
	DU8			0xF3, 0x0F, 0x5A, 0x45, 0x10;
//	MOVQ		RAX, XMM0
	DU8			0x66, 0x48, 0x0F, 0x7E, 0xC0;

	POP			RBP
	RET1		8
}
/**
	@ingroup Math
	@brief Convert single-precision scalar to double-precision.

	@param[in] f	Single-precision scalar.
	@return			Double-precision scalar.
*/
_extern _F32_2_F64 F64 F322F64(F32 f);

asm
{
_F32_RAD_2_DEG::
	PUSH		RBP
	MOV			RBP, RSP

//	MOVSS		XMM0, SF_ARG1[RBP]
	DU8			0xF3, 0x0F, 0x10, 0x45, 0x10;
	MOV			RAX, F32_PI_OVER_180
//	MOVQ		XMM1, RAX
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xC8;
//	MULSS		XMM0, XMM1
	DU8			0xF3, 0x0F, 0x59, 0xC1;
//	MOVQ		RAX, XMM0
	DU8			0x66, 0x48, 0x0F, 0x7E, 0xC0;

	POP			RBP
	RET1		8
}
/**
	@ingroup Math
	@brief Convert single-precision radians to single-precision degrees.

	@param[in] rad	Single-precision angle in radians.
	@return			Single-precision angle in degrees.
*/
_extern _F32_RAD_2_DEG F32 F32Rad2Deg(F32 rad);

asm
{
_F32_DEG_2_RAD::
	PUSH		RBP
	MOV			RBP, RSP

//	MOVSS		XMM0, SF_ARG1[RBP]
	DU8			0xF3, 0x0F, 0x10, 0x45, 0x10;
	MOV			RAX, F32_180_OVER_PI
//	MOVQ		XMM1, RAX
	DU8			0x66, 0x48, 0x0F, 0x6E, 0xC8;
//	MULSS		XMM0, XMM1
	DU8			0xF3, 0x0F, 0x59, 0xC1;
//	MOVQ		RAX, XMM0
	DU8			0x66, 0x48, 0x0F, 0x7E, 0xC0;

	POP			RBP
	RET1		8
}
/**
	@ingroup Math
	@brief Convert single-precision degrees to single-precision radians.

	@param[in] deg	Single-precision angle in degrees.
	@return			Single-precision angle in radians.
*/
_extern _F32_DEG_2_RAD F32 F32Deg2Rad(F32 deg);

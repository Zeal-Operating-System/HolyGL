asm
{
_DOUBLE_2_SINGLE::
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
	@ingroup gfxmath
	@brief Convert double-precision scalar to single-precision.

	@param[in] d	Double-precision scalar.
	@return			Single-precision scalar.
*/
_extern _DOUBLE_2_SINGLE I32 Double2Single(F64 d);

asm
{
_MAT4_MUL_VEC4::
	PUSH		RBP
	MOV			RBP, RSP

	MOV			RAX, SF_ARG1[RBP]
//	MOVAPS		XMM0, [RAX]

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
_extern _MAT4_MUL_VEC4 U0 Mat4MulVec4(CMat4 *m, CVec4 *v, CVec4 *dest);
/* Include this file to rebuild the Math library. */

Option(OPTf_WARN_PAREN, ON);
Option(OPTf_WARN_DUP_TYPES, ON);
Option(OPTf_WARN_UNUSED_VAR, OFF);	// Assembly pulls args from the stack
									// instead of using argument identifiers.
Cd(__DIR__);;

#include "Conversion"
#include "F32"
#include "Vec4"
#include "Vec3"
#include "Mat4"

#define MATH_COMPILED

Option(OPTf_WARN_PAREN, OFF);
Option(OPTf_WARN_DUP_TYPES, OFF);
Option(OPTf_WARN_UNUSED_VAR, ON);

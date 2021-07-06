/* Include this file to rebuild the Math library. */

Option(OPTf_WARN_PAREN, ON);
Option(OPTf_WARN_DUP_TYPES, ON);
Option(OPTf_WARN_UNUSED_VAR, OFF);	// Assembly pulls args from the stack

Cd(__DIR__);;

#include "Vec4"
#include "Vec3"
#include "Mat4"
#include "Conversion"
/* Include this file to rebuild the CosmicGL library. */

Option(OPTf_WARN_PAREN, ON);
Option(OPTf_WARN_DUP_TYPES, ON);

Cd(__DIR__);;

#include "Math/Types"
#include "WAD/Types"
#include "Texture/Types"
#include "Context/Types"	// Global context

//#include "Math/Vec4"
//#include "Math/Vec3"
//#include "Math/Conversion"

#include "Texture/Texture"
#include "Draw/Primitive"
#include "Draw/Texture"
#include "Draw/Rectangle"
#include "Draw/Text"
#include "WAD/WAD"
#include "Texture/TextureImport"
#include "Context/Bind"
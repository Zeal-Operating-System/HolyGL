/* Include this file to rebuild the CosmicGL library. */

Option(OPTf_WARN_PAREN, ON);
Option(OPTf_WARN_DUP_TYPES, ON);

Cd(__DIR__);;

#include "Math/Types"
#include "WAD/Types"
#include "Texture/Types"
#include "Context/Types"	// Global context
#include "Plot/Types"

//#include "Math/MakeMath"

<<<<<<< Updated upstream
#include "WAD/WAD"
=======
#include "WAD/WAD"

>>>>>>> Stashed changes
#include "Texture/Texture"
#include "Texture/TextureLoad"
#include "Texture/TextureSave"

#include "Draw/Primitive"
#include "Draw/Texture"
#include "Draw/Rectangle"
#include "Draw/Text"

#include "Context/Bind"

#define HOLYGL_COMPILED

Option(OPTf_WARN_PAREN, OFF);
Option(OPTf_WARN_DUP_TYPES, OFF);

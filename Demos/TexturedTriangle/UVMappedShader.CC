// This demo only works with halflife.wad in the same directory.

Cd(__DIR__);;
#include "../../Math"
#include "../../WAD"
#include "../../Texture"
#include "../../Shader"
#include "../../Rasterize"

I64 wWC = 80;  		// Window width in columns (~640)
I64 wHC = 45; 		// Window height in rows (~384)
I64 wW = wWC * 8;
I64 wH = wHC * 8; 
I64 wXC = 1;       		// Window x in columns
I64 wYC = 2;       		// Window y in rows
I64 wX = wXC * 8;
I64 wY = (wYC + 4) * 8;

SettingsPush;
WinHorz(wXC, wXC + wWC - 1);
WinVert(wYC, wYC + wHC + 3);
DocClear;

// Framebuffer Initialization
CTex2D frameBuf;
CTex2D depthBuf;

Tex2DInit(&frameBuf, TEX2D_RAW, wW, wH);
Tex2DInit(&depthBuf, TEX2D_DEPTH, wW, wH);

// Colors
CBGR24 cBlack;
cBlack.r = 0; cBlack.g = 0; cBlack.b = 0;
CBGR24 cBlu;
cBlu.r = 0; cBlu.g = 100; cBlu.b = 255;
Tex2DColorFill(&frameBuf, cBlack);
//DrawTriangle(&frameBuf, 64, 0, 64, 64, 128, 32, cBlu);


// Texture Intitalization
CWAD wad;
WADLoad(&wad, "halflife.wad");
CTex2D texture;
Tex2DLoadWAD(&texture, &wad, "!waterblue");
WADFree(&wad);

// Create custom model format that is quite straightforward
class MDL
{
	I64 nTris;
	CTri *tri;
	CVec2 *uv;
};

// Load it with a texture mapped triangle
MDL model;
model.nTris = 1;
model.tri = MAlloc(sizeof(CTri));
model.tri[0].p[0].x = 0.2;
model.tri[0].p[0].y = 0.5;
model.tri[0].p[0].z = 0.5;
model.tri[0].p[1].x = -0.5;
model.tri[0].p[1].y = -0.2;
model.tri[0].p[1].z = -0.5;
model.tri[0].p[2].x = 0.0;
model.tri[0].p[2].y = 0.0;
model.tri[0].p[2].z = 0.0;
model.uv = MAlloc(sizeof(CVec2)*3);
model.uv[0].x = 0.0;
model.uv[0].y = 0.0;
model.uv[1].x = 1.0;
model.uv[1].y = 0.0;
model.uv[2].x = 1.0;
model.uv[2].y = 1.0;

MDL modelB;
modelB.nTris = 1;
modelB.tri = MAlloc(sizeof(CTri));
modelB.tri[0].p[0].x = 1.2;
modelB.tri[0].p[0].y = 0.8;
modelB.tri[0].p[0].z = 0.0;
modelB.tri[0].p[1].x = -0.5;
modelB.tri[0].p[1].y = -0.2;
modelB.tri[0].p[1].z = 0.0;
modelB.tri[0].p[2].x = 0.0;
modelB.tri[0].p[2].y = 0.0;
modelB.tri[0].p[2].z = 0.0;
modelB.uv = MAlloc(sizeof(CVec2)*3);
modelB.uv[0].x = 0.0;
modelB.uv[0].y = 0.0;
modelB.uv[1].x = 2.0;
modelB.uv[1].y = 2.0;
modelB.uv[2].x = 2.0;
modelB.uv[2].y = 0.0;


// We will not create a uniforms buffer, only feed the pointer to the 
// texture we want to sample (no point making a new buffer if that is
// the only uniform we need to feed it).

// Generate a custom shader and test it
CShader shd, shdB;
shd.vertValues = MAlloc(sizeof(I64));
shd.vertValues[0] = SHD_CVEC2; // UV coordinate

shdB.vertValues = MAlloc(sizeof(I64));
shdB.vertValues[0] = SHD_CVEC2;

shd.nVertValues = 1;
shdB.nVertValues = 1;

U0 vShader(CTri *tri, F64 *vertOutBuf, U8 *mdlPtr, U8 *uniforms, I64 iTri, I64 nTris)
{
	MDL *mdl = mdlPtr;
	MemCopy(tri, &mdl->tri[iTri], sizeof(CTri));
	MemCopy(vertOutBuf, &mdl->uv[iTri*3], sizeof(CVec2)*3);
}

U0 fShader(CBGR24 *color, F64 *fragInBuf, U8 *uniforms)
{
	Tex2DSampleNorm(color, uniforms, fragInBuf[0], fragInBuf[1]);
}

U0 fShaderB(CBGR24 *color, F64 *fragInBuf, U8 *uniforms)
{
	color->r = 230;
	color->g = 0;
	color->b = 0;
}

shd.FragShd = &fShader;
shd.VertShd = &vShader;
shdB.FragShd = &fShaderB;
shdB.VertShd = &vShader;

Tex2DDepthReset(&depthBuf);
RenderTris(&shd, &frameBuf, &depthBuf, &model, &texture, 1);
RenderTris(&shdB, &frameBuf, &depthBuf, &modelB, &texture, 1);

while (CharScan() == 0)
{
	//RenderTris(&shd, &frameBuf, &depthBuf, &model, &texture, 1);
	Tex2DDebugDisp(&frameBuf, wX, wY);
	Sleep(1);
}

Tex2DFree(&frameBuf);
Tex2DFree(&depthBuf);
Tex2DFree(&texture);
SettingsPop;
Exit;

/* This demo renders a BSP. */

Cd(__DIR__);;
#include "../../Gr2.CC"
Cd(__DIR__);;

"\n\n\n\n";		// Seperate from annoying warnings

I64 wWC = 90;	// Window width in columns
I64 wHC = 90;	// Window height in rows
I64 wW = wWC * 8;
I64 wH = wHC * 8;
I64 wXC = 1;	// Window x in columns
I64 wYC = 2;	// Window y in rows
I64 wX = wXC * 8;
I64 wY = wYC * 8;

SettingsPush;
WinHorz(wXC, wXC + wWC - 1);
WinVert(wYC, wYC + wHC - 1);
DocClear;

// Prepare framebuffer
CTex2D frameBuf;
Tex2DInit(&frameBuf, TEX2D_RAW, wW, wH);
Tex2DColorFill(&frameBuf, gr_palette[WHITE]);

// Load BSP and render
CWAD doom1;
WADLoad(&doom1, "DOOM1.WAD");

CDoomMap map;
DoomMapLoad(&map, &doom1, "E1M1");

CBGR24 cRed;
cRed.r = 255; cRed.b = cRed.g = 0;

I64 i, a, b, c, d;
for (i = 0; i < map.nLinedefs; i++)
{
	a = map.linedefs[i].startVertex->x >> 32;
	b = map.linedefs[i].startVertex->y >> 32;
	c = map.linedefs[i].endVertex->x >> 32;
	d = map.linedefs[i].endVertex->y >> 32;

	a /= 10;
	b /= 10;
	c /= 10;
	d /= 10;

	a += 200;
	b += 650;
	c += 200;
	d += 650;

	if (a < 0 || b < 0 || c < 0 || d < 0 ||
		a >= wW || b >= wH || c >= wW || d >= wH)
	{
		// One coordinate outside window
	}
	else
	{
		//DrawPoint(&frameBuf, a, b, cRed);
		DrawLine(&frameBuf, a, b, c, d, cRed);
	}
}

// Attempt texture loading
CTex2D patch;
Tex2DLoadWAD(&patch, &doom1, "POSSF2F8");	// As of right now the tex points to the wad

while (CharScan() == 0)
{
	Tex2DDebugDisp(&frameBuf, wX, wY);
	Tex2DDebugDisp(&patch, 200, 200);
	Sleep(1);
}

WADFree(&doom1);
Tex2DFree(&patch);
Tex2DFree(&framebuf);
SettingsPop;
Exit;

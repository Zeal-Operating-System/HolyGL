/* This demo shows how to draw primitives to textures. */

Cd(__DIR__);;
#include "../../Gr2.CC"
Cd(__DIR__);;

I64 wWC = 64;	// Window width in columns (512px)
I64 wHC = 48;	// Window height in rows (384px)
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

// Draw rectangles
DrawRectFill(&frameBuf, 32, 32, 128, 96, gr_palette[CYAN]);

DrawRectVertGradient(&frameBuf, 32, 128, 128, 144, gr_palette[WHITE], gr_palette[LTGRAY], 0.5, 1.0);

DrawRectSoftOutline(&frameBuf, 32, 128, 128, 144, gr_palette[LTGRAY]);

while (CharScan() == 0)
{
	Tex2DDebugDisp(&frameBuf, wX, wY);
	Sleep(1);
}

Tex2DFree(&frameBuf);
SettingsPop;
Exit;

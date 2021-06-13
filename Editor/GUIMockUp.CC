/* Temporary mockup of GUI by drawing all of the elements with hardcoded
positions to figure out what is needed for the UI library. */

Cd(__DIR__);;
#include "../Gr2"
Cd(__DIR__);;

I64 wWC = 80;	// Window width in columns (640px)
I64 wHC = 60;	// Window height in rows (480px)
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
Tex2DColorFill(&frameBuf, palette[C_BCKGND]);

I64 row = 1;
DrawTextButton(&frameBuf, 1, row, "Vertex", 6, NULL, &row);

while (CharScan() == 0)
{
	Tex2DDebugDisp(&frameBuf, wX, wY);
	Sleep(1);
}

Tex2DFree(&frameBuf);
SettingsPop;
Exit;

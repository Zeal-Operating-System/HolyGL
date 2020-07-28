/* UI Component Classes, note that these are not final, they will be heavily 
modified going forward. */

#define UI_CONTAINER	1
#define UI_FRAME		2
#define UI_BUTTON		3

#define UI_LAYOUT_VERT	0
#define UI_LAYOUT_HORZ	1
#define UI_LAYOUT_GRID	2

#define UI_ALIGN_LEFT	0
#define UI_ALIGN_RIGHT	1
#define UI_ALIGN_BOTTOM	2
#define UI_ALIGN_TOP	3

class CUIComponent
{
	I64 type;
	CUIComponent *next, *prev, *parent, *child;
};

class CUIContainer
{
	I64 w, h;
	I64 layout;
};

class CUIFrame : CUIContainer
{
	CTex2D *tex;
};

class CUILabel : CUIComponent
{
	U8 *label;
	U8 *font;
	U16 color;		// Color index from palette
	U16 textLen;	// Length of text, >0 to overide default length
};

class CUIButton : CUIComponent
{
	U8 *description;
	U0 (*EventLMBPressed)();
	U0 (*EventRMBPressed)();
};

U0 DrawButton(CTex2D *tex, I64 col, I64 row, U8 *str, I64 size = 0,
	I64 *nCol = NULL, I64 *nRow = NULL)
{
	if (size == 0)
		size = StrLen(str);

	I64 x = col * FONT_WIDTH;
	I64 y = row * FONT_HEIGHT;

	DrawRectSoftOutline(tex, x - 3, y - 3, x + 1 + size * FONT_WIDTH,
		y + FONT_HEIGHT + 1, gr_palette[DKGRAY]);

	DrawRectVertGradient(tex, x - 2, y - 2, x + 1 + size * FONT_WIDTH,
		y + FONT_HEIGHT + 1, gr_palette[DKGRAY], gr_palette[WHITE]);

	DrawString(tex, x, y, str, gr_palette[BLACK ]);

	if (nCol != NULL)
		*nCol += size + 1;

	if (nRow != NULL)
		*nRow += 2;
}

/* The set of values passed down to aid in drawing subsequent branches. */
class CUITreeStatus
{
	I64 row, col;
	I64 layout;
	CTex2D *tex;
};

/* Recusrively draws UI tree. */
U0 DrawUI(CUIComponent *comp, CUITreeStatus status)
{
	switch (comp->type)
	{
		case UI_CONTAINER:
			break;

		case UI_FRAME:
			//status->tex = comp->tex;
			break;

		case UI_BUTTON:
			break;
	}

	// Draw from lowest leaf up
	if (comp->child != NULL)
		DrawUI(comp->child, status);

	// Draw current component

	// Draw next sibling
	if (comp->next != NULL)
		DrawUI(comp->child, status);
}
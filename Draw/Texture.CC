
/* Draw a mask texture with a chosen color. */
U0 DrawMaskTex(CTex2D *tex, I64 x, I64 y, CTex2D *mask, CBGR24 color)
{
	I64 i, j;
	for (j = 0; j < mask->h; j++)
	{
		for (i = 0; i < mask->w; i++)
		{
			// To tired for this, I'll fix it later
			//if (mask->maskBuf[i + (1 << i))
			//	tex->rawBuf[x + i + (y + j) * tex->w] = color;
		}
	}
}
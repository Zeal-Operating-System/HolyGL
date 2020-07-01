#define TEX2D_RAW 1
#define TEX2D_DEPTH 2
#define TEX2D_WAD 3
#define TEX2D_DXT 4

#define FOURCC_DXT1 0x31545844


class CTex2D
{
	I64 type;			// Type, 0:Uninitialized, 1:Raw, 2:Depth, 3:WAD, 4:DXT
	I64 w;				// Width
	I64 h;				// Height
	I64 mipmaps;
	CBGR24 *rawBuf;		// Raw color buffer
	U8 *wadBuf;			// Buffer for WAD textures
	F64 *depthBuf;		// Depth buffer
	CBGR24 *paletteBuf;	// Palette if compressed format like WAD is used
};

// Initialize texture, see TEX2D_ definitions for type options
U0 Tex2DInit(CTex2D *tex, I64 type, I64 w, I64 h)
{
	tex->w = w;
	tex->h = h;
	tex->type = type;

	switch (type)
	{
		case (TEX2D_RAW):
			tex->rawBuf = MAlloc(4 * tex->w * tex->h);
		break;

		case (TEX2D_DEPTH):
			tex->depthBuf = MAlloc(sizeof(F64) * tex->w * tex->h);
		break;

		case (TEX2D_WAD):
			tex->wadBuf = MAlloc(tex->w * tex->h);
			tex->paletteBuf = MAlloc(4 * 256);
		break;
	}
}

// Free texture from memory
U0 Tex2DFree(CTex2D *tex)
{
	switch (tex->type)
	{
		case TEX2D_RAW:
			Free(tex->rawBuf);
		break;

		case TEX2D_DEPTH:
			Free(tex->depthBuf);
		break;

		case TEX2D_WAD:
			Free(tex->wadBuf);
			Free(tex->paletteBuf);
		break;
	}
}

// Fill texture with color, only works with raw textures
U0 Tex2DColorFill(CTex2D *tex, CBGR24 color)
{
	MemSetU32(tex->rawBuf, color, tex->w * tex->h);
}

U0 Tex2DDepthReset(CTex2D *tex)
{
	F64 depth = 2.0;
	I64 *iDepth = &depth;
	MemSetI64(tex->depthBuf, *iDepth, tex->w * tex->h);
} 

// Renders directly to display buffer. Only for debugging.
U0 Tex2DDebugDisp(CTex2D *tex, I64 x, I64 y)
{
	I64 i, j;
	F64 scaled;
	CBGR24 color;

	for (j = 0; j < tex->h; j++)
	{
		for (i = 0; i < tex->w; i++)
		{
			switch (tex->type)
			{
				case TEX2D_RAW:
					text.fb_alias[(x + i) + (y + j) * GR_WIDTH] 
						= tex->rawBuf[i + j * tex->w];
				break;

				case TEX2D_DEPTH:
					// Scaled to display 0-1024 depth
					scaled = tex->depthBuf[i + j * tex->w] / 4;
					color.r = scaled;
					color.g = scaled;
					color.b = scaled;
					text.fb_alias[(x + i) + (y + j) * GR_WIDTH]
						= color;
				break;

				case TEX2D_WAD:
					text.fb_alias[(x + i) + (y + j) * GR_WIDTH]
						= tex->paletteBuf[tex->wadBuf[i + j * tex->w]];
				break;
			}
		}
	}
}

U0 Tex2DLoadWAD(CTex2D *tex, CWAD *wad, U8 *texName)
{
	I64 texIndex = -1;	// Will be positive if texture is found
	I64 i;
	for (i = 0; i < wad->header[0].nDir; i++)
	{
		if (StrMatch(texName, wad->entries[i].szName) != 0)
		{
			texIndex = i;
			break;
		}
	}
	if (i == -1)
	{
		"[ERROR] TEXTURE %s COULDN'T BE FOUND", texName;
	}
	else
	{
		// Map texture class to WAD file buffer
		CWADTex *wadTex = wad->fileBuf + 
			wad->entries[texIndex].nFilePos;
		// Debug
		// "TEX: %s, W: %d, H: %d", wadTex->szName,
			wadTex->nWidth, wadTex->nHeight;
		// Initialize with WAD format
		Tex2DInit(tex, TEX2D_WAD, wadTex->nWidth, 
			wadTex->nHeight);
		// Copy image bytes (each byte references a palette)
		MemCopy(tex->wadBuf, wad->fileBuf +
			wad->entries[texIndex].nFilePos + wadTex->nOffsets[0], 
			wadTex->nWidth * wadTex->nHeight);
		// After the last byte of the fourth mipmap, there
		// Are two padding bytes, then the palette of triples
		// of bytes (R,G,B)
		U8 *palette = wad->fileBuf +
			wad->entries[texIndex].nFilePos + wadTex->nOffsets[3] + 
			((wadTex->nWidth * wadTex->nHeight) / 64) + 2;
		for (i = 0; i < 256; i++)
		{
			tex->paletteBuf[i].r = palette[i * 3];
			tex->paletteBuf[i].g = palette[(i * 3) + 1];
			tex->paletteBuf[i].b = palette[(i * 3) + 2];
		}
	}
}

// Normalized UV coordinate sampling (0-1)
// This is not filtered (for now)
U0 Tex2DSampleNorm(CBGR24 *col, CTex2D *tex, F64 u, F64 v)
{
	I64 x = u * tex->w;
	I64 y = v * tex->h;
	x %= tex->w;
	y %= tex->h;

	switch (tex->type)
	{
		case TEX2D_RAW:
			MemCopy(col, &tex->rawBuf[x + y * tex->w], sizeof(CBGR24));
		break;

		case TEX2D_WAD:
			MemCopy(col, &tex->paletteBuf[tex->wadBuf[x + y * tex->w]], sizeof(CBGR24));
		break;
	}
}

// Straight pixel UV coordinate sampling, this saves
// steps for rendering BSP which use this method.
U0 Tex2DSampleCoord(CBGR24 *col, CTex2D *tex, I64 u, I64 v)
{
	u %= tex->w;
	v %= tex->h;

	switch (tex->type)
	{
		case TEX2D_RAW:
			MemCopy(col, &tex->rawBuf[u + v * tex->w], sizeof(CBGR24));
		break;

		case TEX2D_WAD:
			MemCopy(col, &tex->paletteBuf[tex->wadBuf[u + v * tex->w]], sizeof(CBGR24));
		break;
	}
}


















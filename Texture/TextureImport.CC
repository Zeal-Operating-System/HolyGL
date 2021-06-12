

U0 Tex2DLoadWAD(CTex2D *tex, CWAD *wad, U8 *texName, U8 *paletteBuf=NULL)
{
	I64 i;
	U8 *palette;
	CWADTex *wadTex;

	// Get lump handle
	CWADFileLump *lump;
	I64 texIndex = WADFindLump(wad, texName, &lump);
	
	if (texIndex < 0)
	{
		"[ERROR] CAN NOT FIND %s TEXTURE IN WAD\n", texName;
		return;
	}

	// Half-Life/Quake Texture
	if (wad->type == WAD_TYPE_HL)
	{		
		// Map texture class to WAD file buffer
		wadTex = wad->fileBuf + lump->hl.filePos;
	
		// Initialize texture with WAD format
		Tex2DInit(tex, TEX2D_WAD, wadTex->width, 
			wadTex->height);

		// Copy image bytes (each byte references the palette)
		MemCopy(tex->wadBuf, wad->fileBuf +
			lump->hl.filePos + wadTex->offsets[0], 
			wadTex->width * wadTex->height);

		// After the last byte of the fourth mipmap, there
		// Are two padding bytes, then the palette of triples
		// of bytes (R,G,B)
		palette = wad->fileBuf +
			lump->hl.filePos + wadTex->offsets[3] + 
			((wadTex->width * wadTex->height) / 64) + 2;

		for (i = 0; i < 256; i++)
		{
			tex->paletteBuf[i].r = palette[i * 3];
			tex->paletteBuf[i].g = palette[(i * 3) + 1];
			tex->paletteBuf[i].b = palette[(i * 3) + 2];
		}

		return;
	}

	CWADPatch *wadPatch;
	U32 *columnOffsets;
	U8 *column;
	U8 columnOffset;
	U8 columnHeight;

	// DOOM Patch
	if (wad->type == WAD_TYPE_DOOM)
	{
		// Map patch class to WAD file buffer
		wadPatch = wad->fileBuf + lump->doom.filePos;

		// Initialize texture with patch format
		Tex2DInit(tex, TEX2D_PATCH, wadPatch->w, wadPatch->h, 
			wadPatch->l, wadPatch->t);

		// Patches usually share palettes
		if (paletteBuf != NULL)
		{
			tex->paletteBuf = paletteBuf;
		}
		else
		{
			tex->paletteBuf = wad->paletteBuf;

			// Make up a test palette
			/*tex->paletteBuf = MAlloc(4 * 256);
			for (i = 0; i < 256; i++)
			{
				tex->paletteBuf[i].r = i;
				tex->paletteBuf[i].g = i;
				tex->paletteBuf[i].b = i;
			}*/
		}

		// Point columnOffsets in texture to column locations in memory
		// In future these will point to addresses in a buffer the texture holds
		for (i = 0; i < wadPatch->w; i++)
		{
			tex->columnOffsets[i] = wad->fileBuf + lump->doom.filePos + wadPatch->columnOffsets[i];
				//&wadPatch[0] + wadPatch->columnOffsets[i];
		}
	}
}



/* BMP Format Header */

class CBMPHeader
{
	U8 szVersion[2];	// Should have 'BM' signature
	U32 nSize;			// File size in bytes
	U32 pad;			// Unused
	U32 nOffset;		// Offset from beginning of file to data;

	/* Info header is right after first header, so it is merged into this class */
	U32 nInfoSize;		// Size of this info part of the header (=40)
	U32 nWidth;			// Image width in pixels
	U32 nHeight;		// Image height in pixels
	U16 nPlanes;		// Number of planes (=1)
	U16 nPxBits;		// Number of bits per pixel. This also indicates palette method:
						// 1-bit:	Monochrome
						// 4-bit:	16-color palette
						// 8-bit:	256-color palette
						// 16-bit:	65565 colors
						// 24-bit:	16M colors
	U32 nCompression;	// Type of compression:
						// 0: BI_RGB	no-compression
						// 1: BI_RLE8	8-bit RLE encoding
						// 2: BI_RLE4 	4-bit RLE encoding
	U32 nImageSize;		// Size of image if compressed, (can =0 if not compressed)
	U32 nXPxPerM;		// Horizontal resolution, pixels per meter
	U32 nYPxPerM;		// Vertical resolution, pixels per meter
	U32 nColors;		// Number of actually used colors (ie. 256 for 8-bit color)
	U32 nImptColors;	// Number of important colors (=0 for all) 
};

U0 Tex2DLoadBMP(CTex2D *tex, U8 *fname, Bool mask = FALSE, 
	Bool invert = TRUE)
{
	U8 *bmp = FileRead(fname);

	// Map locations in buffer;
	CBMPHeader *info = bmp;

	//if (StrMatch(bmp->szVersion, "BM") == 0)
	if (info->szVersion[0] != 'B' && info->szVersion[1] != 'M')
	{
		"[ERROR] %s IS NOT A VALID BMP FILE!\n", fname;
		return;
	}

	if (info->nPxBits != 24 || info->nCompression != 0)
	{
		"[ERROR] %s IS NOT A 24-BIT UNCOMPRESSED BMP!\n", fname;
		return;
	}
	
	// Each pixel is 3 bytes in 24-bit BMP, however CTex2D uses CBGR24
	// which contains a final padding bit (32-bits per pixel). The rows
	// also increment in the opposite direction.
	I64 imgPointer = bmp + info->nOffset;
	I64 xPx, yPx;
	
	// For checking if a color channel is greater than 0 for masks
	U8 channel;

	if (mask)
	{
		// Initialize with MASK format for 1-bit non-compressed color
		Tex2DInit(tex, TEX2D_MASK, info->nWidth, info->nHeight);

		for (yPx = 0; yPx < info->nHeight; yPx++)
		{
			for (xPx = 0; xPx < info->nWidth; xPx++)
			{
				channel = *(imgPointer + (xPx + info->nWidth * 
					(info->nHeight - yPx)) * 3);

				if ((channel == 0 && invert = TRUE) ||
					(channel > 0 && invert = FALSE))
				{
					tex->maskBuf[(xPx + info->nWidth * yPx) / 8] | (1 << (xPx % 8));
				}
			}
		}
	}
	else
	{
		// Initialize with RAW format for 24-bit non-compressed color
		Tex2DInit(tex, TEX2D_RAW, info->nWidth, info->nHeight);
	
		for (yPx = 0; yPx < info->nHeight; yPx++)
		{
			for (xPx = 0; xPx < info->nWidth; xPx++)
			{
				MemCopy(&tex->rawBuf[xPx + info->nWidth * yPx], 
					imgPointer + (xPx + info->nWidth * (info->nHeight - yPx)) 
					* 3, 3);
			}
		}
	}
}


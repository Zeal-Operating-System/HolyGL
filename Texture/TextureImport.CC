/* WAD Format Header */


#define WAD_MAXTEXTURENAME 16
#define WAD_MIPLEVELS 4


class CWADHeader
{
	U8 szVersion[4];// Should be WAD2/WAD3
	I32 nDir;		// Number of directory entries
	I32 nDirOffset;	// Offset into directory
};


class CWADEntry
{
	I32 nFilePos;					// Relative offset
	I32 nDiskSize;					// Size of file
	I32 nSize;						// Uncompressed size
	I8 nType;						// Type of entry
	I8 bCompression;				// 0 if none
	I16 pad;
	U8 szName[WAD_MAXTEXTURENAME];
};


class CWADTex
{
	U8 szName[WAD_MAXTEXTURENAME];
	U32 nWidth, nHeight;
	U32 nOffsets[WAD_MIPLEVELS];	// Relative offset
};


// CWAD is used as a handle for the texture loading
// functions while the necessary textures are copied, 
// then it is closed.
class CWAD
{
	U8 *fileBuf;		// Temporary loaded file
	CWADHeader *header;
	CWADEntry *entries;	// Entry array
	CWADTex *textures;	// Texture array
};


// Load WAD file  
U0 WADLoad(CWAD *wad, U8 *fname)
{
	wad->fileBuf = FileRead(fname);

	// Map locations in buffer
	wad->header = wad->fileBuf;
	wad->entries = wad->fileBuf + 
		wad->header[0].nDirOffset;
}


// Free WAD file from memory
U0 WADFree(CWAD *wad)
{
	Free(wad->fileBuf);
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



/* BMP Format Header */

class CBMPHeader
{
	U8 szVersion[2];// Should have 'BM' signature
	U32 nSize;		// File size in bytes
	U32 pad;		// Unused
	U32 nOffset;	// Offset from beginning of file to data;

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


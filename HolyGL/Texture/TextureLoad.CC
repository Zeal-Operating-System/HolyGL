
/**
	@ingroup GLTex2D
	@brief Loads texture from Half-Life WAD file.

	@param[in,out] tex		Unitialized texture to load into.
	@param[in]     wad		WAD file to load from.
	@param[in]     texName	Name of texture lump in WAD file to load.
*/
U0 GLTex2DLoadWAD(CGLTex2D *tex, CGLWAD *wad, U8 *texName)
{
	I64 i, texIndex;
	U8 *pixel, *palette;
	CGLWADFileLump *lump;
	CGLWADTex *wadTex;
	CBGR24 color;

	// Get lump handle
	texIndex = GLWADFindLump(wad, texName, &lump);
	
	if (texIndex < 0)
	{
		"[ERROR] CAN NOT FIND %s TEXTURE IN WAD %s\n", texName, wad->name;
		return;
	}

	// Map texture class to WAD file buffer
	wadTex = wad->fileBuf + lump->filePos;

	GLTex2DInit(tex, GL_TEX2D_RAW, wadTex->w, wadTex->h);

	// Map pixel array
	pixel = wad->fileBuf + lump->filePos + wadTex->offsets[0];
	
	// After the last byte of the fourth mipmap, there are two padding bytes,
	// then a palette of 24-bit color (R, G, B).
	palette = wad->fileBuf + lump->filePos + wadTex->offsets[3] +
		wadTex->w * wadTex->h / 64 + 2;	
		// 1/64 here is the relative size of 4th mip map

	color.pad = 255;	// Alpha

	for (i = 0; i < wadTex->w * wadTex->h; i++)
	{
		color.r = palette[pixel[i] * 3];
		color.g = palette[pixel[i] * 3 + 1];
		color.b = palette[pixel[i] * 3 + 2];
		
		tex->rawBuf[i] = color;

		// Full blue pixels are transparent
		if (color == 0xFF0000FF)
			tex->rawBuf[i].pad = 0;
	}
}

/**
	@ingroup GLTex2D
	@brief Loads BMP image.

	Supports standard uncompressed 24-bit color BMP images.

	@param[in,out] tex			Uninitialized texture to load into.
	@param[in]     fname		File name of BMP.
	@param[in]     monochrome	(Optional) Loads red channel as alpha to make
								a mask texture.
*/
U0 GLTex2DLoadBMP(CGLTex2D *tex, U8 *fname, Bool monochrome = FALSE)
{
	I64 imgPointer,
		xPx, yPx;
	U8	*bmp, alpha, *bmpBuf; 

	bmp = FileRead(fname);

	// Map locations in buffer;
	CGLBMPHeader *info = bmp;

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
	imgPointer = bmp + info->nOffset;
	bmpBuf = bmp + info->nOffset;

	// Initialize with RAW format for 24-bit non-compressed color
	GLTex2DInit(tex, GL_TEX2D_RAW, info->nWidth, info->nHeight);

	if (!monochrome)
	{
		for (yPx = 0; yPx < info->nHeight; yPx++)
		{
			for (xPx = 0; xPx < info->nWidth; xPx++)
			{
				tex->rawBuf[xPx + info->nWidth * yPx] =
					*(imgPointer + 
					(xPx + info->nWidth * (info->nHeight - yPx - 1)) * 3);
			}
		}
	}
	else
	{
		for (yPx = 0; yPx < info->nHeight; yPx++)
		{
			for (xPx = 0; xPx < info->nWidth; xPx++)
			{
				// We only need an alpha byte, which is an inverted color byte
				// on the BMP (black = TRUE, white = FALSE).
				alpha = bmpBuf[(xPx + info->nWidth *
					(info->nHeight - yPx - 1)) * 3];
				alpha = 255 - alpha;	// Invert
				tex->rawBuf[xPx + info->nWidth * yPx].pad = alpha;
			}
		}
	}
	

	Free(bmp);
}


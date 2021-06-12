#define TEX2D_RAW	1
/* This format is a CBGR24 rawBuf array of size w * h in row-major order. */

#define TEX2D_DEPTH	2
/* This format is a F64 depthBuf array of size w * h in row-major order. */

#define TEX2D_WAD	3
/* This format is a U8 wadBuf array of size w * h in row-major order. Each byte
points to a color in the CBGR24 paletteBuf array which has a max size of 255. */

#define TEX2D_MASK	4
/* This format is a U8 array of size w * h in row-major order. Each byte
corresponds to one pixel, and is either true or false. Mask textures can also
be used for stencils buffers. */

#define TEX2D_DXT	5
/* This format is not yet implemented. */

#define TEX2D_PATCH	6
/* This format is optimized for column rendering. */

#define TEX2D_COLUMN_MAJOR_TYPES 6 	// Column major types >= this, row major <

#define FOURCC_DXT1 0x31545844

/* Posts are a vertical segment of opaque pixels in a column. There may be
multiple posts in a column, one after the other. The end of a column
is signified by a post with an offset of 255. This class is variable
length, and is meant to be mapped to a memory location for easy reading. */
class CTex2DPost
{
	U8 yOffset;		// Offset from start of column or previous post
					// This is 255 if signaling end of column
	U8 length;		// Length in pixels/bytes
	U8 pad;
	U8 pixels[256];	// Variable size, maximum length of a column is 255
	// Technically there is a pad byte here but pixels is variable size
}

class CTex2D
{
	I64 type;			// Type, see TEX2D_ macros above
	I64 w;				// Width
	I64 h;				// Height
	I64 l;				// Left offset if applicable (for use with sprite textures)
	I64 t;				// Top offset if applicable (for use with sprite textures)
	I64 mipmaps;		// Mipmaps, not currently being used

	/* Buffers */

	CBGR24 *rawBuf;		// Raw color buffer. Size: w * h
	U8 *wadBuf;			// WAD buffer. Size: w * h, pixels point to paletteBuf
	U8 *maskBuf;		// Mask buffer. Size: w * h, pixels are TRUE/FALSE
	F64 *depthBuf;		// Depth buffer. Size: w * h, pixels are depth.
	CBGR24 *paletteBuf;	// Palette if compressed format like WAD/Patch/DXT is used
	U8 *patchBuf;		// Buffer of posts (see CTex2DPost).
	I64 *columnOffsets;	// Testing: Pointer to column wherever in memory that may be
						// Offset in patchBuf to start of each column. Size: w
};

// Initialize texture, see TEX2D_ definitions for type options
U0 Tex2DInit(CTex2D *tex, I64 type, I64 w, I64 h, I64 l = 0, I64 t = 0)
{
	tex->w = w;
	tex->h = h;
	tex->l = l;
	tex->t = t;
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

		case (TEX2D_PATCH):
			tex->columnOffsets = MAlloc(tex->w * sizeof(I64));
			// Let the loader handle this for now.
			// tex->patchBuf = CAlloc(tex->w * tex->h);
			// paletteBuf pointer may be changed on the fly for lighting
		break;

		case (TEX2D_MASK):
			tex->wadBuf = CAlloc((tex->w * tex->h) / 8);
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

		case TEX2D_PATCH:
			Free(tex->columnOffsets);
			// Do not free palette buffer as this may be shared
		break;
	}
}

// Fill texture with color, only works with raw textures
U0 Tex2DColorFill(CTex2D *tex, CBGR24 color)
{
	MemSetU32(tex->rawBuf, color, tex->w * tex->h);
}

// Fill depth texture with far depth
U0 Tex2DDepthReset(CTex2D *tex)
{
	F64 depth = 2.0;
	I64 *iDepth = &depth;
	MemSetI64(tex->depthBuf, *iDepth, tex->w * tex->h);
} 

// Renders directly to display buffer. Only for debugging.
U0 Tex2DDebugDisp(CTex2D *tex, I64 x, I64 y)
{
	// i, j = sampling x, y position on texture
	I64 i, j;
	
	// For scaling depth buffer to 8 bit color
	F64 scaled;
	CBGR24 color;

	// For patch textures
	U8 *position;	// Current position in memory reading pixels from
	CTex2DPost *post;	// Mapped to memory to read post pixels easier

	if (tex->type < TEX2D_COLUMN_MAJOR_TYPES)
	{
		// Iterate through x then y for row major types
		for (j = 0; j < tex->h; j++)
		{
			for (i = 0; i < tex->w; i++)
			{
				switch (tex->type)
				{
					case TEX2D_RAW:
						text.fb_alias[(x + i) + (y + j) * GR_WIDTH] 
							= tex->rawBuf[i + j * tex->w];
					break;

					case TEX2D_DEPTH:
						// Scaled to display 0-1024 depth
						scaled = tex->depthBuf[i + j * tex->w] / 4;
						color.r = scaled;
						color.g = scaled;
						color.b = scaled;
						text.fb_alias[(x + i) + (y + j) * GR_WIDTH] = color;
					break;

					case TEX2D_WAD:
						text.fb_alias[(x + i) + (y + j) * GR_WIDTH]
							= tex->paletteBuf[tex->wadBuf[i + j * tex->w]];
					break;
				}

			}
		}
	
	}
	else
	{
		// Only one column-major type: CTEX2D_PATCH

		for (i = 0; i < tex->w; i++)
		{
			// Get start of column position from columnOffsets pointer array
			position = tex->columnOffsets[i];
		
			// Map post to start of column for easy post reading
			post = position;

			// Starting column offset. Each post is offset from the previous
			I64 columnOffset = 0;

			// Post with offset 255 is last post in column and is empty.
			while (post->yOffset != 255)
			{
				columnOffset = post->yOffset;

				// Draw each pixel in post to framebuffer
				for (j = 0; j < post->length; j++)
				{
					text.fb_alias[(x + i) +
						// Here j is current y in this post, offset is added 
						(y + j + columnOffset) * GR_WIDTH]
						= tex->paletteBuf[post->pixels[j]];
				}

				//columnOffset += post->length;

				// Next post starts 1 pad byte after end of previous buffer
				position += post->length + 4; // 3 starting bytes + last pad
				post = position;
			}
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

		case TEX2D_PATCH:
			MemCopy(col, &tex->paletteBuf[tex->patchBuf[x * tex->h + y]], sizeof(CBGR24));
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

		case TEX2D_PATCH:
			MemCopy(col, &tex->paletteBuf[tex->patchBuf[u * tex->h + v]], sizeof(CBGR24));
		break;
	}
}


















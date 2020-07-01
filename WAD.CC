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
	U8 *fileBuf;		// Temporary loaded file
	CWADHeader *header;
	CWADEntry *entries;	// Entry array
	CWADTex *textures;	// Texture array
};


// Load WAD file  
U0 WADLoad(CWAD *wad, U8 *fname)
{
	wad->fileBuf = FileRead(fname);

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
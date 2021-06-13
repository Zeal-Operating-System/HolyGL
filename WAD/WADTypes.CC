/* WAD File Format */


class CWADInfo
{
	U8 type[4];		// Type of WAD, like IWAD/PWAD/WAD3
	I32 nLumps;		// Number of lumps
	I32 lumpOffset;	// Offset to lump
};

class CWADFileLumpDoom
{
	I32 filePos;		// Relative offset
	I32 size;
	U8 name[8];
};

class CWADFileLumpHL
{
	I32 filePos;		// Relative offset
	I32 diskSize;		// Size of lump (compressed)
	I32 size;			// Uncompressed lump size
	I8 type;
	I8 compression;		// 0 if none
	I16 pad;
	U8 name[16];
};

// Not a WAD data structure.
// Used to access elements of an unspecified lump pointer.
union CWADFileLump
{
	CWADFileLumpDoom doom;
	CWADFileLumpHL hl;
};


/* DOOM BSP */


class CWADThing
{
	I16 x;
	I64 y;
	I16 angle;
	I16 type;
	I16 flags;
};

// Linedefs make up the shape of the map.
class CWADLinedef
{
	I16 v1;				// Vertex 1
	I16 v2;				// Vertex 2
	I16 flags;
	I16 type;
	I16 sectorTag;
	I16 frontSidedef;	// Sidedefs hold texture data for each side of linedef
	I16 backSidedef;
};

// Textures for one side of a wall (linedef).
class CWADSidedef
{
	I16 xOffset;		// Texture x offset
	I16 yOffset;		// Texture y offset
	U8 upperTex[8];		// These are found in TEXTUREx lumps
	U8 lowerTex[8];
	U8 middleTex[8];
	I16 sectorFaces;	// Sector this sidedef faces
};

class CWADVertex
{
	I16 x;
	I16 y;
};

class CWADSeg
{
	I16 v1;
	I16 v2;
	I16 angle;
	I16 linedef;
	I16 direction;
	I16 offset;
};

class CWADSector
{
	I16 floorHeight;
	I16 ceilingHeight;
	U8 floorTex[8];
	U8 ceilingTex[8];
	I16 lightLevel;
	I16 flags;
	I16 tag;
};

// A subsector is a range of linedefs, and forms part or all
// of a single sector. Subsectors are always convex.
class CWADSubSector
{
	I16 nLinedefs;
	I16 firstLinedef;
};

class CWADNode
{
	I16 x;
	I16 y;
};


/* DOOM Flats, Sprites, and Patches */

// This is variable length, do not allocate but instead map to loaded buffer
class CWADPatch
{
	U16 w;
	U16 h;
	I16 l;					// Offsets to shift origin point when drawing
	I16 t;
	U32 columnOffsets[1];	// Array of offsets (relative to header) of size
							// width. If columns are identical they may point 
							// to the same column in the file.
};

/* Posts are a vertical segment of opaque pixels in a column. There may be
multiple posts in a column, one after the other. The end of a column
is signified by a post with an offset of 255. This class is variable
length, and is meant to be mapped to a memory location for easy reading. */
class CWADPost
{
	U8 yOffset;		// Offset from start of column. This is 255 if
					// signaling end of column.
	U8 length;		// Length is bytes/pixels
	U8 pad;
	U8 pixels[1];	// Size is equal to length. Each pixel points to a color
					// in the color palette.
	// Technically another U8 pad comes here but pixels is variable length
};

// Header to TEXTUREx lumps
class CWADTexHeader
{
	I32 nTextures;	// Number of textures in TEXTUREx lump
	I32 offsets[1];	// Offsets to CWADMapTex's from start of this class
};

// Patch
class CWADMapPatch
{
	I16 l;			// Left offset (x origin)
	I16 t;			// Top offset (y origin)
	I16 patch;		// Patch number listed in PNAMES lump
	I16 stepdir;	// Obsolete, ignored by all DOOM versions
	I16 colormap;	// Obsolete, ignored by all DOOM versions
};

// Texture in TEXTUREx lump. A texture is made up of multiple patches
class CWADMapTex
{
	U8 name[8];
	I32 masked;					// Boolean
	I16 width;
	U16 height;
	U8 columnDir[4];			// Obsolete, ignored by all DOOM versions
	I16 patchCount;				// Number of patches that make up texture
	CWADMapPatch patches[1];	// Size dependent on patchCount
};

/* Half Life Texture */

class CWADTexHL
{
	U8 name[16];
	U32 w;
	U32 h;
	U32 offsets[4];	// Relative offset to each mip map
};
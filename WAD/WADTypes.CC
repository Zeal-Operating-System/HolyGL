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

// Linedefs make up the shape of the map.
class CWADLinedef
{
	I16 startVertex;
	I16 endVertex;
	I16 flags;
	I16 type;
	I16 sectorTag;
	I16 frontSidedef;
	I16 backSidedef;
};

// Textures for one side of a wall (linedef).
class CWADSidedef
{
	I16 xOffset;
	I16 yOffset;
	U8 upperTex[8];
	U8 lowerTex[8];
	U8 middleTex[8];
	I16 sectorFaces;	// Sector this sidedef faces
};

class CWADVertex
{
	I32 x;
	I32 y;
};

class CWADSegs
{
	I16 startVertex;
	I16 endVertex;
	I16 angle;
	I16 linedef;
	I16 direction;
	I16 offset;
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

/* Half Life Texture */

class CWADTex
{
	U8 name[16];
	U32 width;
	U32 height;
	U32 offsets[4];	// Relative offset to each mip map
};
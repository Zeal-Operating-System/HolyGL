class CDoomVertex
{
	I64 x;
	I64 y;
};

class CDoomBBox
{
	I64 x1;
	I64 y1;
	I64 x2;
	I64 y2;
};

/**
	A sector is a 2D section of the map.
*/
class CDoomSector
{
	I64 floorH;		// Floor height
	I64 ceilingH;	// Ceiling height

	CTex2D *floorTex;
	CTex2D *ceilingTex;

	I64 lightLevel;
	I64 special;
	I16 tag;

	I64 nLines;				// Total lines associated with sector
	//CDoomLine **lines;		// Array of line pointers
	CDoomBBox blockBox[4];	// Bounding box for height changes
};

/**
	A side contains the textures applied to a line (wall).
*/
class CDoomSide
{
	I64 xOffset;
	I64 yOffset;

	CTex2D *upperTex;
	CTex2D *lowerTex;
	CTex2D *middleTex;

	CDoomSector *sectorFacing;
};


/**
	Lines make up the shape of the map. A one sided wall is a solid wall,
	while a two sided wall is a boundary between sectors.
*/
class CDoomLine
{
	CDoomVertex *v1;
	CDoomVertex *v2;
	CDoomSide *frontSide;
	CDoomSide *backSide;
};

/**
	Clipped line that is used to make up subsectors.
*/
class CDoomSeg
{

}

/**
	List of segs indicating some or all visible walls that define a convex
	BSP leaf.
*/
class CDoomSubSector
{
	CDoomSector *sector;
	I64 nLines;
	I64 firstLine;
};

#define NF_SUBSECTOR 0x8000

/**
	Splits level down a line. On either side of the line is either another
	child node or a BSP leaf (subsector). 
*/
class CDoomNode
{
	I64 x;			// Partition start coordinates
	I64 y;
	I64 xChange;	// Change from start to end of partition line
	I64 yChange;
	CDoomBBox rightBox;
	CDoomBBox leftBox;
	CDoomNode *rightChild;
	CDoomNode *leftChild;
};


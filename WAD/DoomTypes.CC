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

class CDoomSidedef
{
	I64 xOffset;
	I64 yOffset;

	CTex2D *upperTex;
	CTex2D *lowerTex;
	CTex2D *middleTex;

	// CDoomSector *sectorFacing;
};

class CDoomLinedef
{
	CDoomVertex *startVertex;
	CDoomVertex *endVertex;
	I16	flags;
	I16 type;
	I16 sectorTag;
	CDoomSidedef *frontSidedef;
	CDoomSidedef *backSidedef;
}

class CDoomSector
{
	I64 floorHeight;
	I64 ceilingHeight;

	CTex2D *floorTex;
	CTex2D *ceilingTex;

	I64 lightLevel;
	I16 type;
	I16 tag;

	// Generated:
	I64 nLinedefs;	// Total linedefs associated with sector
	I64 *lines;		// List of linedef associated with sector
	I64 blockbox;	// Coordinates of rectangular box around sector,
					// box enlarged to align with blockmap.
}

class CDoomNode
{
	I64 x;			// Partition start coordinates
	I64 y;
	I64 xChange;	// Change from start to end of partition line
	I64 yChange;
	CDoomBBox *rightBox;
	CDoomBBox *leftBox;
	CDoomNode *rightChild;
	CDoomNode *leftChild;
}


class CDoomVertex
{
	I64 x;
	I64 y;
}

class CDoomSector
{
	I64 floorHeight;
	I64 ceilingHeight;

	I16 floorTex;
	I16 ceilingTex;
	I16 lightLevel;
	I16 special;
	I16 tag;

	I64 linecount;
	I64 *lines;
}


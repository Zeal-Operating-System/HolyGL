
// Temporary includes
Cd(__DIR__);;
#include "WAD"


class CDoomMap
{
	U8 *fileBuf;	// Map buffer in memory to free later

	CWADThing *things;
	I32 nThings;
	CWADLinedef *linedefs;
	I32 nLinedefs;
	CWADSidedef *sidedefs;
	I32 nSidedefs;
	CWADVertex *vertexes;
	I32 nVertexes;
	CWADSegs *segs;
	I32 nSegs;
	CWADSubSector *subSectors;
	I32 nSubSectors;
	CWADNode *nodes;
	I32 nNodes;
};

// Find index of lump with name
I64 DoomWADFindLump(U8 *lumps, I64 startLump, I64 nLumps, 
	U8 *name)
{
	CWADFileLumpDoom *lump;	// Current lump being checked

	I64 i;
	for (i = startLump; i < nLumps; i++)
	{
		lump = lumps + (i * sizeof(CWADFileLumpDoom));


		if (StrNICompare(name, lump->name, 8) == 0)
		{
			return i;	// Lump with matching name found
		}
	}

	return -1;	// Not found
}

U0 DoomMapLoad(CDoomMap *map, U8 *wadName, U8 *mapName)
{
	map->fileBuf = FileRead(wadName);

	CWADInfo *info;
	CWADFileLumpDoom *lumps;

	// Map locations in buffer
	info = map->fileBuf;
	lumps = map->fileBuf + info->lumpOffset;

	// Find lump with map name
	I64 mapLump = DoomWADFindLump(lumps, 0, info->nLumps, mapName);
	if (mapLump < 0)
	{
		"[ERROR] %s map not found in %s\n", mapName, wadName;
		return;
	}

	// New lumps pointer increased and total length decreased
	// to only search for lumps for this map
	//mapLump++;
	//I64 nLumps = info->nLumps - mapLump;

	"Map Found: %d\n", mapLump;
}

U0 DoomMapFree(CDoomMap *map)
{
	Free(map->fileBuf);
}


CDoomMap map;
DoomMapLoad(&map, "DOOM1.WAD", "E1M1");
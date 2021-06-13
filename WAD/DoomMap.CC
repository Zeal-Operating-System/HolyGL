
#define HTT_DOOM_TEX	420		// Hash table type for texture hash
#define TEX_TABLE_SIZE	256		// This is overkill and we can get a more accuracte

class CHashDoomTex : CHash
{
	CTex2D tex;
};

class CDoomMap
{
	I64 pad;

	CDoomVertex *vertexes;
	I64 nVertexes;

	CDoomSide *sides;
	I64 nSides;

	CDoomLine *lines;
	I64 nLines;

	// CHashTable *texTable;	// Hash table for quick access of textures
};

/**
	Load blockmap from WAD into DOOM map.
*/
U0 DoomMapLoadBlockmap(CDoomMap *map, CWAD *wad, I64 lumpIndex)
{

}

/**
	@internal
	Load vertexes from WAD into DOOM map. See CDoomVertex.

	@param CDoomMap *map	Map to load into.
	@param CWAD *wad		WAD to load from.
	@param I64 lumpIndex	Index of VERTEXES lump in WAD.
*/
U0 DoomMapLoadVertexes(CDoomMap *map, CWAD *wad, I64 lumpIndex)
{
	CWADFileLumpDoom *lump = &wad->doomLumps[lumpIndex];

	if (StrNICompare("VERTEXES", lump->name, 8) != 0)
	{
		"[ERROR] EXPECTED VERTEXES LUMP INDEX DOES NOT POINT TO A 
			VERTEXES LUMP\n";
		return;
	}

	// Map WAD vertex array
	CWADVertex *verts = wad->fileBuf + lump->filePos;

	// Calculate and update number of vertexes
	I64 nVerts = lump->size / sizeof(CWADVertex);
	map->nVertexes = nVerts;

	// Allocate memory for map vertexes
	map->vertexes = MAlloc(sizeof(CDoomVertex) * nVerts);

	// Load I16 vertexes to I64 fixed point
	I32 xIntermediate;
	I32 yIntermediate;
	I64 i;
	for (i = 0; i < nVerts; i++)
	{
		// Load I16 to I32 to preserve sign at MSB when shifting 32 bits left
		// for I64 fixed point conversion.
		xIntermediate = verts[i].x;
		yIntermediate = verts[i].y;

		map->vertexes[i].x = xIntermediate << 32;
		map->vertexes[i].y = yIntermediate << 32;
	}
}

/**
	@internal
	Load sectors from WAD into DOOM map.
*/
U0 DoomMapLoadSectors(CDoomMap *map, CWAD *wad, I64 lumpIndex)
{

}

/**
	@internal
	Load sides from WAD into DOOM map. See CDoomSide.

	@param CDoomMap *map	Map to load into.
	@param CWAD *wad		WAD to load from.
	@param I64 lumpIndex	Index of SIDEDEFS lump in WAD.
*/
U0 DoomMapLoadSides(CDoomMap *map, CWAD *wad, I64 lumpIndex)
{
	CWADFileLumpDoom *lump = &wad->doomLumps[lumpIndex];

	if (StrNICompare("SIDEDEFS", lump->name, 8) != 0)
	{
		"[ERROR] EXPECTED SIDEDEFS LUMP INDEX DOES NOT POINT TO A 
			SIDEDEFS LUMP\n";
		return;
	}

	// Map WAD sidedef array
	CWADSidedef *sidedefs = wad->fileBuf + lump->filePos;

	// Calculate and update number of sidedefs
	I64 nSidedefs = lump->size / sizeof(CWADSidedef);
	map->nSides = nSidedefs;

	// Allocate memory for map sides
	map->sides = MAlloc(sizeof(CDoomSide) * nSidedefs);

	I64 i;
	for (i = 0; i < nSidedefs; i++)
	{
		map->sides[i].xOffset = sidedefs[i].xOffset;
		map->sides[i].yOffset = sidedefs[i].yOffset;

		/* Todo - Handle wall texture loading */
	}
}

/**
	@internal
	Load lines from WAD into DOOM map. See CDoomLine.

	@param CDoomMap *map	Map to load into.
	@param CWAD *wad		WAD to load from.
	@param I64 lumpIndex	Index of LINEDEFS lump in WAD.
*/
U0 DoomMapLoadLines(CDoomMap *map, CWAD *wad, I64 lumpIndex)
{
	CWADFileLumpDoom *lump = &wad->doomLumps[lumpIndex];

	if (StrNICompare("LINEDEFS", lump->name, 8) != 0)
	{
		"[ERROR] EXPECTED LINEDEFS LUMP INDEX DOES NOT POINT TO A 
			LINEDEFS LUMP\n";
		return;
	}

	// Map WAD linedefs array
	CWADLinedef *linedefs = wad->fileBuf + lump->filePos;

	// Calculate and update number of linedefs
	I64 nLinedefs = lump->size / sizeof(CWADLinedef);
	map->nLines = nLinedefs;

	// Allocate memory for map linedefs
	map->lines = MAlloc(sizeof(CDoomLine) * nLinedefs);

	I64 i;
	for (i = 0; i < nLinedefs; i++)
	{
		map->lines[i].v1 = &map->vertexes[linedefs[i].v1];
		map->lines[i].v2 = &map->vertexes[linedefs[i].v2];

		map->lines[i].frontSide = &map->sides[linedefs[i].frontSidedef];
		map->lines[i].backSide = &map->sides[linedefs[i].backSidedef];
	}
}

/**
	@internal
	Builds sector line lists and subsector numbers. Finds block bounding
	boxes for sectors.

	@param [IN/OUT] map	Map to modify.
*/
U0 DoomMapGroupLines(CDoomMap *map)
{
	// Look up sector number for each subsector
	
}

/**
	Load DOOM map from WAD file.

	@param CDoomMap *map	Uninitialized map to load into.
	@param CWAD *wad		WAD to load from.
	@param U8 *mapName		Map from WAD to load (max length is 8 characters).
*/
U0 DoomMapLoad(CDoomMap *map, CWAD *wad, U8 *mapName)
{
	I64 mapLumpIndex = WADFindLump(wad, mapName, NULL);

	if (mapLumpIndex < 0)
	{
		"[ERROR] UNABLE TO FIND MAP %s IN WAD\n", mapName;
		return;
	}

	// Prepare texture hash table
	// map->texTable = HashTableNew(TEX_TABLE_SIZE);

	//I64 thingsIndex = mapLumpIndex + 1;

	I64 vertexesIndex = mapLumpIndex + 4;
	DoomMapLoadVertexes(map, wad, vertexesIndex);

	I64 sidedefsIndex = mapLumpIndex + 3;
	DoomMapLoadSides(map, wad, sidedefsIndex);
	
	I64 linedefsIndex = mapLumpIndex + 2;
	DoomMapLoadLines(map, wad, linedefsIndex);

	//I64 segsIndex = mapLumpIndex + 5;
	//I64 subsectorsIndex = mapLumpIndex + 6;
	//I64 rejectIndex = mapLumpIndex + 7;
	//I64 blockmapIndex = mapLumpIndex + 8;
}

/**
	Free DOOM map.

	@param [IN/OUT]	map	Map to free.
*/
U0 DoomMapFree(CDoomMap *map)
{
	// Free(map->vertexes);
	// Free(map->lines);
	// Free(map->sides);
}
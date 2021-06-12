
class CDoomMap
{
	I64 pad;

	CDoomVertex *vertexes;
	I64 nVertexes;
	CDoomLinedef *linedefs;
	I64 nLinedefs;
};

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
		// Load I16 to I32 to preserve sign when shifting 32 bits
		// right for I64 fixed point conversion.
		xIntermediate = verts[i].x;
		yIntermediate = verts[i].y;

		map->vertexes[i].x = xIntermediate << 32;
		map->vertexes[i].y = yIntermediate << 32;
	}

	"%d VERTEXES\n", nVerts;
}

U0 DoomMapLoadLinedefs(CDoomMap *map, CWAD *wad, I64 lumpIndex)
{
	CWADFileLumpDoom *lump = &wad->doomLumps[lumpIndex];

	if (StrNICompare("LINEDEFS", lump->name, 8) != 0)
	{
		"[ERROR] EXPECTED LINEDEFS LUMP INDEX DOES NOT POINT TO A 
			LINEDEFS LUMP\n";
		return;
	}

	// Map WAD linedefs array
	CWADLinedef *linedefs = wad->fileBuf + lump->filePos;

	// Calculate and update number of linedefs
	I64 nLinedefs = lump->size / sizeof(CWADLinedef);
	map->nLinedefs = nLinedefs;

	// Allocate memory for map linedefs
	map->linedefs = MAlloc(sizeof(CDoomLinedef) * nLinedefs);

	I64 i;
	for (i = 0; i < nLinedefs; i++)
	{
		map->linedefs[i].startVertex = 
			&map->vertexes[linedefs[i].startVertex];

		map->linedefs[i].endVertex =
			&map->vertexes[linedefs[i].endVertex];

		/*"V1 %d: %d, %d | V2 %d: %d, %d\n", 
			linedefs[i].startVertex, 
			map->vertexes[linedefs[i].startVertex].x,
			map->vertexes[linedefs[i].startVertex].y,
			
			linedefs[i].endVertex,
			map->vertexes[linedefs[i].endVertex].x,
			map->vertexes[linedefs[i].endVertex].y;*/
	}
	"%d LINEDEFS\n", nLinedefs;
}

U0 DoomMapLoad(CDoomMap *map, CWAD *wad, U8 *mapName)
{
	I64 mapLumpIndex = WADFindLump(wad, mapName, NULL);

	if (mapLumpIndex < 0)
	{
		"[ERROR] UNABLE TO FIND MAP %s IN WAD\n", mapName;
		return;
	}

	//I64 thingsIndex = mapLumpIndex + 1;

	I64 vertexesIndex = mapLumpIndex + 4;
	DoomMapLoadVertexes(map, wad, vertexesIndex);
	
	I64 linedefsIndex = mapLumpIndex + 2;
	DoomMapLoadLinedefs(map, wad, linedefsIndex);

	//I64 sidedefsIndex = mapLumpIndex + 3;

	//I64 segsIndex = mapLumpIndex + 5;
	//I64 subsectorsIndex = mapLumpIndex + 6;
	//I64 rejectIndex = mapLumpIndex + 7;
	//I64 blockmapIndex = mapLumpIndex + 8;
}
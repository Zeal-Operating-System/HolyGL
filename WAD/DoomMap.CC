
class CDoomMap
{
	U8 *fileBuf;	// Map buffer in memory to free later

	CWADThing *things;
	I32 nThings;
	CWADLinedef *linedefs;
	I32 nLinedefs;
	CWADSidedef *sidedefs;
	I32 nSidedefs;
	CWADVertex *vertices;
	I32 nVertices;
	CWADSegs *segs;
	I32 nSegs;
	CWADSubSector *subSectors;
	I32 nSubSectors;
	CWADNode *nodes;
	I32 nNodes;
}

U0 DoomMapLoad(CDoomMap *map, U8 *fname)
{
	U8 map->fileBuf = FileRead(fname);

	CWADInfo *info;
	CWADFileLumpDoom *lump;

	// Map locations in buffer
	info = map->fileBuf;

	I64 i;
	for (i = 0; i < info->nLumps; i++)
	{
		lump = map->fileBuf + info->lumpOffset +
			(i * sizeof(CWADFileLumpDoom));
		
		// Print lump name
		"Lump found: %s\n", lump->name;

		// Todo - Sort into CDoomMap
	}
}

U0 DoomMapFree(CDoomMap *map)
{
	Free(map->fileBuf);
}
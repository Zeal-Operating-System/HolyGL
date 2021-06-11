
#define WAD_TYPE_HL		1
#define WAD_TYPE_DOOM	2
#define HTT_LUMP		69	// Hash Table Type for lump hash

// Hash for quick access of lump by name
class CHashLump : CHash
{
	CWADFileLump *lump;		// Pointer to lump
							// Access with lump->doom/lump->hl
	I64 lumpIndex;			// Index of lump in WAD
};

class CWAD
{
	U8 *fileBuf;
	CWADInfo *info;				// Type, size, pointer to lumps
	CWADFileLumpDoom *doomLumps;// Quick access doom lumps
	CWADFileLumpHL	*hlLumps;	// Quick access half life lumps
	I64 type;					// WAD type
	CHashTable *lumpTable;		// Hash table for quick access of
								// lumps by name.
	CHashLump *hashLumps;		// Hashes for lumpTable.
};

// Load WAD file
U0 WADLoad(CWAD *wad, U8 *fname)
{
	wad->fileBuf = FileRead(fname);

	// Map locations in buffer
	wad->info = wad->fileBuf;
	
	// Allocate hash table
	wad->lumpTable = HashTableNew(wad->info->nLumps);	// Table
	wad->hashLumps = CAlloc(wad->info->nLumps *			// Hashes
		sizeof(CHashLump));

	I64 i;

	// Check version and proceed accordingly
	U32 *type = wad->info->type;	// Cast to U32

	//           'WAD3': Half Life      'WAD2': Quake
	if (*type == 0x33444157 || *type == 0x32444157)
	{
		wad->type = WAD_TYPE_HL;

		// Map lumps to buffer
		wad->hlLumps = wad->fileBuf + wad->info->lumpOffset;
			
		for (i = 0; i < wad->info->nLumps; i++)
		{
			// Copy name to hash
			wad->hashLumps[i].str = CAlloc(17); // 16 + NULL
			MemCopy(wad->hashLumps[i].str, 
				wad->hlLumps[i].name, 16);

			// Set hash type
			wad->hashLumps[i].type = HTT_LUMP;

			// Link lump to hash
			wad->hashLumps[i].lump = &wad->hlLumps[i];

			// Set lump index in hash to this index
			wad->hashLumps[i].lumpIndex = i;

			// Add hash to table
			HashAdd(&wad->hashLumps[i], wad->lumpTable);
		}
		return;
	}
	
	//           'IWAD': Doom Internal  'PWAD': Doom Patch
	if (*type == 0x44415749 || *type == 0x44415750)
	{
		wad->type = WAD_TYPE_DOOM;

		// Map lumps to buffer
		wad->doomLumps = wad->fileBuf + wad->info->lumpOffset;

		for (i = 0; i < wad->info->nLumps - 10; i++)
		{
			// Copy name to hash
			wad->hashLumps[i].str = CAlloc(9); // 8 + NULL
			MemCopy(wad->hashLumps[i].str, 
				wad->doomLumps[i].name, 8);

			// Set hash type
			wad->hashLumps[i].type = HTT_LUMP;

			// Link lump to hash
			wad->hashLumps[i].lump = &wad->doomLumps[i];

			// Set lump index in hash to this index
			wad->hashLumps[i].lumpIndex = i;

			// Add hash to table
			HashAdd(&wad->hashLumps[i], wad->lumpTable);
		}
		return;
	}

	// Type not recognized
	U64 typeStr = 0;
	MemCopy(&typeStr, wad->type, 4);
	"[ERROR] %s WAD TYPE NOT RECOGNIZED: %s\n", typeStr;
}

// Finds lump with name and points **lump at it.
// Returns index of lump, or -1 if lump not found.
I64 WADFindLump(CWAD *wad, U8 *name, CWADFileLump **lump = NULL)
{
	CHashLump *lumpHash;
	lumpHash = HashFind(name, wad->lumpTable, HTT_LUMP);

	if (lumpHash != NULL)
	{
		if (lump != NULL)
			*lump = lumpHash->lump;

		return lumpHash->lumpIndex;
	}
	else
	{
		return -1;
	}
}

// Free WAD file from memory
U0 WADFree(CWAD *wad)
{
	Free(wad->hashLumps);
	Free(wad->lumpTable->body);
	Free(wad->lumpTable);
	Free(wad->fileBuf);
}


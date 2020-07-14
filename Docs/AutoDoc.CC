/* This program automatically generates DolDoc documentation from templates
from Templates/ and comments from code. */

#define LINE_NOTHING 		0
#define LINE_SECTION 		1 	// For start of code section, ie: /*** MATH ***/
#define LINE_SIMPLEDESC		2	// Simple description, ie: /* Adds two numbers. */
#define LINE_FUNCTION		3	// Function line, ie: I64 Add(I64 a, I64 b)
#define LINE_CLASS			4	// Class line, ie class CVec2
#define LINE_ADVDESC		5	// Advanced description, ie: /** Description or **
#define LINE_HEADER			6	// Description header, ie: /**** This program does...

class CFuncDoc
{
	U8 *szSimpleDesc;	// Simple function description
	U8 *szFunction;		// Function
	U8 *szAdvDesc;		// Advanced function description
};

class CSectionDoc
{
	U8 *szSection;			// Section name
	CFuncDoc *functions;	// List of functions
};

class CFileDoc
{
	CSectionDoc *sections;
	CFuncDoc *functions;
};

class CDocLine
{
	U8 *line;
	CDocLine *next;
};


Cd(__DIR__);;


/* Breaks string into tokens based on the delimiter and returns strings as CDocLines. */
I64 StrTok(U8 *str, U8 delim, CDocLine *tokens)
{
	I64 tokBeg = 0;
	I64 tokEnd; 
	I64 strLen = StrLen(str);
	I64 tokLen = 0;

	CDocLine *cToken; // Current token
	CDocLine *temp;
	cToken = tokens; // Note that tokens must already be CAlloc'd

	if (strLen > 1)
	{
		for (tokEnd = 2; tokEnd < strLen; tokEnd++)
		{
			if (str[tokEnd] == delim || tokEnd == strLen - 1)
			{
				tokLen = tokEnd - tokBeg;
				cToken->line = CAlloc(tokLen);
				MemCopy(cToken->line, str + tokBeg, tokLen);
				
				cToken->next = CAlloc(sizeof(CDocLine));
				temp = cToken->next;
				cToken = temp;
				tokBeg = tokEnd + 1;	// Skip delimiter
			}
		}
		Free(cToken);	// Final token is an empty tokens->next
		return 1;	// Tokens were extracted
	}
	else
	{
		return 0;	// No tokens to extract
	}
}


/* Removes all white space from a string. */
U0 StrRemoveSpace(U8 *str)
{
	I64 outputCursor = 0; 	// Cursor for current position in final string
	I64 readCursor;		// Cursor for current position in string to read from
	U8 copyChar;	// Temporary storage for copying char
	I64 strLen = StrLen(str);

	for (readCursor = 0; readCursor < strLen; readCursor++)
	{
		if (str[readCursor] != ' ' && str[readCursor] != '\n' &&
			str[readCursor] != '\t')
		{
			copyChar = str[readCursor];
			str[outputCursor] = copyChar;
			outputCursor++;
		}

	}

	// Make the final character null
	if (outputCursor <= strLen && strLen > 0)
	{
		str[outputCursor] = NULL;
	}
}


/* Print document lines starting from the given line. */
U0 PrintDocLines(CDocLine *docline)
{
	if (docline != NULL && docline->line != NULL)
	{
		"%s\n",docline->line;
	}
	else
	{
		return;
	}
	while (docline->next != NULL)
	{
		docline = docline->next;
		if (docline->line != NULL)
		{
			"%s\n",docline->line;
		}
	}
}

/* Free document lines from a given line onwards. */
U0 FreeDocLines(CDocLine *docline)
{
	CDocLine *prevDocLine;

	while (docline->next != NULL && docline != NULL)
	{
		prevDocLine = docline;
		docline = docline->next;
		if (prevDocLine->line != NULL)
		{
			Free(prevDocLine->line);
		}
		Free(prevDocLine);
	}
	if (docline != NULL)
	{
		if (docline->line != NULL)
		{
			Free(docline->line);
		}
		Free(docline);
	}
}

/* Checks for doc function on line, adds docs and returns last line pointer. */
CDocLine *CheckAndAddAutoDocs(CDocLine *line)
{
	// Safety checks
	if (line == NULL || line->line == NULL)
	{
		return line->next;
	}
	I64 strLen = StrLen(line->line);
	if (strLen < 16)	// A fair minimum number of characters for a line to be a doc function
	{
		return line->next;
	}

	U8 *str = StrNew(line->line);
	CDocLine *tokens = CAlloc(sizeof(CDocLine));

	// Check if it is a function
	StrRemoveSpace(str);
	strLen = StrLen(str);
	if (str[0] == '[' && str[strLen-1] == ']')
	{
		// Remove brackets
		str[strLen-1] = NULL;
		str += 1;
		"INSTRUCTION: %s\n", str;

		if (StrTok(str, ',', tokens))
		{
			"ARGS:\n";
			PrintDocLines(tokens);
		} 
		else
		{
			"[ERROR] NO ARGS FOUND!\n";
		}
		str--;
		Free(str);
		return line->next;
	}

	FreeDocLines(tokens);
	Free(str);
	return line->next;
}

/* Generates documentation given a template file. */
U0 GenerateDoc(CDirEntry *template)
{
	"$FG,5$Generating Doc: %s$FG$\n", template->name;

	// If generated doc already exists then delete it
	// Del(template->name);

	I64 tempSize, strLen;
	U8 *tempBuf = FileRead(template->full_name, &tempSize);	
	I64 lineBeg = 0;
	I64 lineEnd = 0;

	CDocLine *finalDoc = CAlloc(sizeof(CDocLine));
	CDocLine *firstLine = finalDoc;	// Save first line as starting point of the chain

	// Read template buffer line by line
	for (lineEnd = 0; lineEnd < tempSize; lineEnd++)
	{
		if (tempBuf[lineEnd] == '\n')
		{
			// Copy line into CDocLine
			strLen = lineEnd - lineBeg;
			finalDoc->line = MAlloc(strLen);
			finalDoc->next = CAlloc(sizeof(CDocLine));
			MemCopy(finalDoc->line, tempBuf + lineBeg, strLen);

			// Update cursors
			finalDoc = CheckAndAddAutoDocs(finalDoc);
			lineBeg = lineEnd + 1;
		}

	}

	// Remove final CDocLine as it is empty.

	// vPrintDocLines(firstLine);
	FreeDocLines(firstLine);
	Free(tempBuf);
	
}

/* Scans Templates/ and calls GenerateDoc for each template. */
U0 AutoDoc()
{
	Cd("Templates");
	CDirEntry *templates = FilesFind(FUF_JUST_DD);
	Cd("..");

	// Skips first two entries, which are "." and ".."
	CDirEntry *entry = templates->next;

	while (entry->next != NULL)
	{
		entry = entry->next;
		GenerateDoc(entry);
	}
	DirEntryDel(templates);
	DirEntryDel(entry);
}

AutoDoc;
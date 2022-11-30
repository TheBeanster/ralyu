#include "ral_file.h"

#include "ral_memory.h"
#include <stdio.h>

#include "ral_cli.h"



Ral_TextFile* Ral_LoadTextFile(const char* const filename)
{
	if (!filename)
	{
		RalCLI_ERROR("Filename is null!");
		return NULL;
	}

	RalCLI_DEBUGLOG("Loading text file \"%s\"", filename);

	Ral_TextFile* textfile = Ral_ALLOC_TYPE(Ral_TextFile);

	FILE* f = fopen(filename, "r");
	if (!f)
	{
		RalCLI_ERROR("The file \"%s\" could not open!", filename);
		Ral_FREE(textfile);
		return NULL;
	}

	textfile->filename = _strdup(filename);



	// Figure out the size of the file
	int length = 0;
	while (!feof(f))
	{
		char c = fgetc(f);
		length++;
	}
	textfile->length = length;

	textfile->buffer = Ral_MALLOC(length + 1);

	// Read the text in the file
	rewind(f);
	int i = 0;
	while (!feof(f))
	{
		char c = fgetc(f);
		textfile->buffer[i] = c;
		i++;
	}

	fclose(f);

	return textfile;
}



void Ral_DestroyTextFile(Ral_TextFile* const textfile)
{
	Ral_FREE(textfile->filename);
	Ral_FREE(textfile->buffer);
	Ral_FREE(textfile);
}

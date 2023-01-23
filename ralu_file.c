#include "ralu_file.h"

#include "ralu_memory.h"
#include <stdio.h>



char* Ral_LoadFileString(const char* const filename)
{
	if (!filename)
	{
		//CONSOLE_ERROR("Filename is null!\n");
		return NULL;
	}

	//CONSOLE_LOG("Loading file \"%s\"\n", filename);

	FILE* file = fopen(filename, "r");
	if (!file)
	{
		//CONSOLE_ERROR("The file could not open!\n");
		return NULL;
	}



	int length = 0;

	// Figure out the size of the file
	while (!feof(file))
	{
		fgetc(file);
		length++;
	}

	char* filebuffer = Ral_MALLOC(length + 1);

	// Read the file into the buffer
	rewind(file);
	int pos = 0;

	while (!feof(file))
	{
		char c = fgetc(file);
		filebuffer[pos] = c;
		pos++;
	}

	filebuffer[pos] = '\0';

	fclose(file);

	return filebuffer;
}

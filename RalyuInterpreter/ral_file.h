#pragma once

#include "ral_stdinclude.h"



typedef struct
{
	char* filename;

	char* buffer;
	int length;
} Ral_TextFile;



Ral_TextFile* Ral_LoadTextFile(
	const char* const filename
);


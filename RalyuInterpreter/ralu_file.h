#pragma once

#include "ralu_stdinclude.h"



typedef struct
{
	char* filename;

	char* buffer;
	int length;
} Ral_TextFile;



Ral_TextFile* Ral_LoadTextFile(
	const char* const filename
);

void Ral_DestroyTextFile(Ral_TextFile* const textfile);

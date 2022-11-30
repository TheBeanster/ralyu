#pragma once

#include "ral_stdinclude.h"
#include "ral_file.h"

#include "ral_lexer.h"



typedef struct
{
	Ral_TextFile*	file;

	const char*		buffer;
	int				length;

	Ral_Statement*	statements;
	int				numstatements;
} Ral_SourceUnit;



Ral_SourceUnit* Ral_CreateSourceUnit(
	const Ral_TextFile* const textfile
);


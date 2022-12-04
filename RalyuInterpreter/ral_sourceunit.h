#pragma once

#include "ralu_stdinclude.h"
#include "ralu_file.h"
#include "ralu_list.h"

#include "ral_lexer.h"



typedef struct
{
	Ral_TextFile*	file;

	const char*		buffer;
	int				length;

	Ral_Statement*	statements;
	int				numstatements;

	Ral_List		errormessages;
} Ral_SourceUnit;



Ral_SourceUnit* Ral_CreateSourceUnit(
	const Ral_TextFile* const textfile
);

void Ral_DestroySourceUnit(
	Ral_SourceUnit* const sourceunit
);

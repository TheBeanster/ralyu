#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"



typedef struct Ral_SourceUnit
{
	Ral_LISTLINKS(Ral_SourceUnit);

	Ral_Statement* statements;
	int numstatements;
} Ral_SourceUnit;



void Ral_DestroySourceUnit(
	Ral_SourceUnit* const sourceunit
);

void Ral_PrintSourceUnit(
	const Ral_SourceUnit* const sourceunit
);

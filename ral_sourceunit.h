#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"



typedef struct Ral_SourceUnit
{
	Ral_LISTLINKS(Ral_SourceUnit);

	struct Ral_Statement* statements;
	int numstatements;
} Ral_SourceUnit;



void Ral_DestroySourceUnit(
	Ral_SourceUnit* const sourceunit
);

void Ral_PrintSourceUnit(
	const Ral_SourceUnit* const sourceunit
);



Ral_SourceUnit* Ral_LoadSourceUnitStatements(
	struct Ral_State* const state,
	Ral_List* const statements,
	struct Ral_Statement* const begin
);

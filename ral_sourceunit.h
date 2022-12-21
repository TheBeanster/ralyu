#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_state.h"



typedef struct Ral_SourceUnit
{
	Ral_LISTLINKS(Ral_SourceUnit);

	struct Ral_Statement* statements;
	int numstatements;
} Ral_SourceUnit;



Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string
);

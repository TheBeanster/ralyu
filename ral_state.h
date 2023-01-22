#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_sourceunit.h"



typedef struct Ral_State
{
	Ral_List sourceunits;

	Ral_List global_variables;
	Ral_List functions;

	// Pointer to a string literal with an error message
	const char* errormsg;
} Ral_State;



Ral_State* Ral_CreateState();

void Ral_DestroyState(
	Ral_State* const state
);



Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string
);

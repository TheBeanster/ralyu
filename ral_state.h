#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_sourceunit.h"



typedef struct Ral_State
{
	Ral_List sourceunits;

	Ral_List global_types;
	Ral_List global_variables;
} Ral_State;



Ral_State* Ral_CreateState();

void Ral_DestroyState(
	Ral_State* const state
);



Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string
);

#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"



typedef struct Ral_State
{
	Ral_List sourceunits;
} Ral_State;



Ral_State* Ral_CreateState();

void Ral_DestroyState(Ral_State* const state);



struct Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string
);

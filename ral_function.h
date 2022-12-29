#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_state.h"
#include "ral_sourceunit.h"
#include "ral_object.h"



typedef struct Ral_Function
{
	Ral_LISTLINKS(Ral_Function);
	
	Ral_SourceUnit* source;
} Ral_Function;



Ral_Object* Ral_CallFunction(
	Ral_State* const state,
	const Ral_Function* const function,
	const Ral_List* const arguments
);

#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_object.h"
#include "ral_state.h"


typedef struct Ral_Variable
{
	Ral_LISTLINKS(Ral_Variable);

	char* name;
	Ral_Object* obj;
} Ral_Variable;



Ral_Variable* Ral_CreateVariable(
	const char* const name,
	Ral_Object* const value
);

Ral_Variable* Ral_DeclareVariable(
	Ral_List* const variable_list,
	Ral_Variable* const variable
);

Ral_Variable* Ral_GetVariable(
	Ral_State* const state,
	Ral_List* const local_variables,
	const char* const name
);

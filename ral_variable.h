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

Ral_Variable* Ral_GetOrDeclareVariable(
	Ral_State* const state,
	Ral_List* const local_variables,
	const char* const name
);



Ral_Object* Ral_GetGlobal(Ral_State* const state, const char* const name);

Ral_Variable* Ral_SetGlobal(Ral_State* const state, const char* const name, const Ral_Object* const obj);

#define Ral_SetGlobalBool(state, name, boolean) Ral_SetGlobal(state, name, Ral_CreateBoolObject(boolean))
#define Ral_SetGlobalNumber(state, name, number) Ral_SetGlobal(state, name, Ral_CreateNumberObject(number))

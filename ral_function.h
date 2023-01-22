#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_state.h"
#include "ral_object.h"



typedef Ral_Object(*Ral_CFunction)(Ral_State*, Ral_List*);



typedef struct Ral_FunctionArgument
{
	Ral_LISTLINKS(Ral_FunctionArgument);
	char* name;
} Ral_FunctionArgument;

typedef struct Ral_Function
{
	Ral_LISTLINKS(Ral_Function);
	
	struct Ral_Statement* bodystart; // Statement 'function'
	struct Ral_Statement* bodyend; // Statement 'end'
	Ral_CFunction* cfunction; // Pointer to C function

	char* name; // Function name
	int numarguments; // The number of arguments
	char** argumentnames; // The names of the agruments
} Ral_Function;



Ral_Function* Ral_CreateFunction(
	struct Ral_Statement* bodystart,
	struct Ral_Statement* bodyend,
	const char* const name,
	const Ral_List* const arguments
);

void Ral_DestroyFunction(
	Ral_Function* const function
);

Ral_Bool Ral_DeclareFunction(
	Ral_State* const state,
	Ral_Function* const function
);

void Ral_RemoveFunction(
	Ral_State* const state,
	Ral_Function* const function
);

Ral_Function* Ral_GetFunction(
	Ral_State* const state,
	const char* const name
);

Ral_Object* Ral_CallFunction(
	Ral_State* const state,
	const Ral_Function* const function,
	const Ral_List* const arguments
);

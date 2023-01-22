#include "ral_function.h"

#include "ralu_memory.h"
#include "ralu_string.h"
#include <stdio.h>

#include "ral_lexer.h"
#include "ral_execute.h"
#include "ral_variable.h"



Ral_Function* Ral_CreateFunction(
	struct Ral_Statement* bodystart,
	struct Ral_Statement* bodyend,
	const char* const name,
	const Ral_List* const arguments
)
{
	printf("Creating function: \"%s\"\n", name);
	Ral_Function* function = Ral_ALLOC_TYPE(Ral_Function);

	function->name = _strdup(name);
	function->bodystart = bodystart;
	function->bodyend = bodyend;
	function->numarguments = arguments->itemcount;
	function->argumentnames = Ral_CALLOC(arguments->itemcount, sizeof(char*));
	Ral_FunctionArgument* iterator = arguments->begin;
	for (int i = 0; i < arguments->itemcount; i++)
	{
		function->argumentnames[i] = _strdup(iterator->name);
		iterator = iterator->next;
	}
	function->cfunction = NULL;
	return function;
}



void Ral_DestroyFunction(Ral_Function* const function)
{
	Ral_FREE(function->argumentnames);
	Ral_FREE(function->name);
	Ral_FREE(function);
}



Ral_Bool Ral_DeclareFunction(Ral_State* const state, Ral_Function* const function)
{
	Ral_Function* overload = Ral_GetFunction(state, function->name);
	if (overload)
	{
		Ral_RemoveFunction(state, overload);
	}
	Ral_PushBackList(&state->functions, function);
	return Ral_TRUE;
}



void Ral_RemoveFunction(Ral_State* const state, Ral_Function* const function)
{
	Ral_UnlinkFromList(&state->functions, function);
	Ral_DestroyFunction(function);
}



Ral_Function* Ral_GetFunction(
	Ral_State* const state,
	const char* const name
)
{
	Ral_Function* iterator = state->functions.begin;
	while (iterator)
	{
		if (strcmp(name, iterator->name) == 0) return iterator;
		iterator = iterator->next;
	}
	return NULL;
}



Ral_Object* Ral_CallFunction(
	Ral_State* const state,
	const Ral_Function* const function,
	const Ral_List* const arguments
)
{
	if (!function->bodystart)
	{
		// Function body is emtpy
		// This is a C function

		return (function->cfunction)(state, arguments);
	}
	
	Ral_Statement* current_statement = function->bodystart;
	Ral_Object* return_object = NULL;
	Ral_List stack = { 0 }; // Stack of statements?
	Ral_List local_variables = { 0 };

	Ral_Object* iterator = arguments->begin;
	for (int i = 0; i < arguments->itemcount; i++)
	{
		Ral_DeclareVariable(&local_variables, Ral_CreateVariable(function->argumentnames[i], iterator));
		iterator = iterator->next;
	}

	while (current_statement)
	{
		current_statement = Ral_ExecuteStatement(
			state,
			current_statement,
			&stack,
			&local_variables,
			Ral_TRUE,
			&return_object
		);
	}

	return return_object;
}





Ral_Function* Ral_DeclareCFunction(
	Ral_State* const state,
	Ral_CFunction* const cfunction,
	const char* const name
)
{
	//printf("Linking C function: \"%s\"\n", name);
	Ral_Function* function = Ral_ALLOC_TYPE(Ral_Function);

	function->name = _strdup(name);
	function->bodystart = NULL;
	function->bodyend = NULL;
	function->cfunction = cfunction;

	Ral_DeclareFunction(state, function);
	return function;
}

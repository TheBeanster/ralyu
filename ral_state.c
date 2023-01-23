#include "ral_state.h"

#include "ralu_memory.h"
#include <stdio.h>
#include "ralu_string.h"

#include "ral_lexer.h"



Ral_State* Ral_CreateState()
{
	Ral_State* state = Ral_ALLOC_TYPE(Ral_State);
	state->errormsg = "";
	return state;
}



void Ral_DestroyState(Ral_State* const state)
{
	Ral_ClearList(&state->sourceunits, &Ral_DestroySourceUnit);
	//Ral_ClearList(&state->global_variables, &Ral_DestroyObject);
	Ral_FREE(state);
}



Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string
)
{
	printf("Loading source string into a sourceunit.\n");

	Ral_List statements = { 0 };
	if (!Ral_ReadSourceStatements(&statements, string))
	{
		printf("Oh no this bad\n");
		return NULL;
	}

	Ral_SourceUnit* source = Ral_CreateSourceUnit(&statements);
	Ral_ClearList(&statements, NULL);

	if (!source)
	{
		printf("Could not create the sourceunit!\n");
		return NULL;
	}

	Ral_PushBackList(&state->sourceunits, source);

	return source;
}

#include "ral_state.h"

#include "ralu_memory.h"
#include <stdio.h>
#include "ralu_string.h"

#include "ral_lexer.h"



Ral_State* Ral_CreateState()
{
	Ral_State* state = Ral_ALLOC_TYPE(Ral_State);
	return state;
}



void Ral_DestroyState(Ral_State* const state)
{
	Ral_SourceUnit* su_iterator = state->sourceunits.begin;
	while (su_iterator)
	{
		

		su_iterator = su_iterator->next;
	}
}



Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string
)
{
	Ral_List statements = { 0 };
	if (!Ral_ReadSourceStatements(&statements, string))
	{
		printf("Oh no this bad\n");
		return NULL;
	}

	return NULL;
}

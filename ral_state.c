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
	Ral_SourceUnit* sourceunit = Ral_ALLOC_TYPE(Ral_SourceUnit);
	if (!Ral_ParseSourceUnit(sourceunit, string))
	{
		printf("Could not parse source code!\n");
		Ral_FREE(sourceunit);
		return NULL;
	}



	return sourceunit;
}

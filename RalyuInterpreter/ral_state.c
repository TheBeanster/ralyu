#include "ral_state.h"

#include "ralu_memory.h"
#include "ral_state.h"



Ral_State* Ral_CreateState(const Ral_SourceUnit* const mainsource)
{
	Ral_State* state = Ral_ALLOC_TYPE(Ral_State);
	state->mainsource = mainsource;

	// Set the execution location to the start of the mainsource
	state->current_sourceunit = mainsource;
	Ral_SetStateExecutionPosition(state, mainsource, 0);
	return state;
}



Ral_Bool Ral_SetStateExecutionPosition(
	Ral_State* const state,
	const Ral_SourceUnit* const sourceunit,
	const int statementid
)
{
	state->current_sourceunit = sourceunit;
	state->current_statementid = statementid;
	if (statementid >= sourceunit->numstatements) return Ral_FALSE;
	state->current_statement = &sourceunit->statements[statementid];
	return Ral_TRUE;
}
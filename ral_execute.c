#include "ral_execute.h"



void Ral_ExecuteGlobalSourceUnit(
	Ral_State* const state,
	const Ral_SourceUnit* const source
)
{
	if (!state) return;
	if (!source) return;
	if (source->numstatements <= 0) return;
	if (!source->statements) return;

	Ral_Statement* current_statement = &source->statements[0];
	Ral_Object* return_object = NULL;

	while (current_statement)
	{
		current_statement = Ral_ExecuteStatement(state, current_statement, &state->global_variables, &return_object);
	}
}



Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const scope_variables,
	Ral_Object** const return_object
)
{
	return NULL;
}

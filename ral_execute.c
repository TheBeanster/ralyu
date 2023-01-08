#include "ral_execute.h"

#include "ral_logging.h"
#include "ral_object.h"
#include "ral_expression.h"



Ral_Object* Ral_ExecuteGlobalSourceUnit(
	Ral_State* const state,
	const Ral_SourceUnit* const source
)
{
	if (!state) return NULL;
	if (!source) return NULL;
	if (source->numstatements <= 0) return NULL;
	if (!source->statements) return NULL;

	Ral_Statement* current_statement = &source->statements[0];
	Ral_Object* return_object = NULL;

	while (current_statement)
	{
		current_statement = Ral_ExecuteStatement(
			state,
			current_statement,
			&state->global_variables,
			Ral_TRUE,
			&return_object
		);
	}

	return return_object;
}



Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const scope_variables,
	const Ral_Bool global,
	Ral_Object** const return_object
)
{
	printf(" - Executing statement -\n");

	switch (statement->type)
	{



	case Ral_STATEMENTTYPE_EXPRESSION:
	{
		
	}
		break;



	default:
		printf("Invalid statement type\n");
		return NULL;
		break;
	}

	if (statement->index + 1 >= statement->parentsource->numstatements) return NULL; // Reached end of sourceunit
	return &statement->parentsource->statements[statement->index + 1];
}

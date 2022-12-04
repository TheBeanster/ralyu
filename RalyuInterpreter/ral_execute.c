#include "ral_execute.h"

#include "ralu_memory.h"

#include "ral_expression.h"
#include "ral_cli.h"



void Ral_ExecuteSource(const Ral_SourceUnit* const sourceunit)
{
	RalCLI_DEBUGLOG("Executing sourceunit");

	Ral_State* state = Ral_CreateState(sourceunit);
	if (!state) return;

	Ral_Bool continue_executing = Ral_TRUE;
	while (continue_executing)
	{
		// Execute the current statement
		
		Ral_ExecuteStatement(state, &state->current_sourceunit->statements[state->current_statementid]);

		// Find what the next statement executed should be

		// Go to next statement
		continue_executing = Ral_SetStateExecutionPosition(state, sourceunit, state->current_statementid + 1);
	}

	Ral_FREE(state);
}



Ral_Bool Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement
)
{
	// The number of tokens in a statement should always be more than 1 so this should be safe
	// If this crashes then something with the lexer has created an empty, useless statement
	RalCLI_DEBUGLOG("Executing statement on line %i", statement->tokens[0].linenum);
	Ral_PrintStatementTokens(statement);

	switch (statement->type)
	{

	case Ral_STATEMENTTYPE_EXPRESSION: {}
		Ral_Expression* expr = Ral_CreateExpression(statement, 0, statement->numtokens);
		
		Ral_DestroyExpression(expr);
		break;

	default:
		break;
	}

	return Ral_TRUE;
}

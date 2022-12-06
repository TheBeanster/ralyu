#include "ral_execute.h"

#include "ralu_memory.h"

#include "ral_expression.h"
#include "ral_cli.h"



Ral_State* Ral_CreateState(const Ral_SourceUnit* const mainsource)
{
	Ral_State* state = Ral_ALLOC_TYPE(Ral_State);
	state->mainsource = mainsource;

	return state;
}





void Ral_ExecuteSource(const Ral_SourceUnit* const sourceunit)
{
	RalCLI_DEBUGLOG("Executing sourceunit");

	if (!sourceunit) return;
	if (sourceunit->numstatements <= 0) return;
	if (!sourceunit->statements) return;

	Ral_Statement* current_statement = &sourceunit->statements[0];
	Ral_State* state = Ral_CreateState(sourceunit);
	if (!state) return;

	while (current_statement)
	{
		// Execute the current statement
		current_statement = Ral_ExecuteStatement(state, current_statement);
	}

	Ral_FREE(state);
}



Ral_Statement* Ral_ExecuteStatement(
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
		
		Ral_Object* expr_result = Ral_EvaluateExpression(state, expr);
		if (!expr_result)
			return NULL;

		Ral_DestroyExpression(expr);
		break;

	default:
		break;
	}

	return NULL;
}

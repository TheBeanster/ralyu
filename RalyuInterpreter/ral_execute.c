#include "ral_execute.h"

#include "ralu_memory.h"

#include "ral_expression.h"
#include "ral_cli.h"
#include "ral_sourceunit.h"



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

	Ral_List local_variables = { 0 };
	Ral_Object* return_object = NULL;

	while (current_statement)
	{
		// Execute the current statement
		current_statement = Ral_ExecuteStatement(state, current_statement, &local_variables, &return_object);
		if (return_object) break;
	}

	Ral_FREE(state);

	RalCLI_DEBUGLOG("Return object ptr = %p", return_object);
}



static Ral_Object* build_and_eval_expression(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_Statement* const statement,
	const int begin,
	const int end
)
{
	Ral_Expression* expr = Ral_CreateExpression(statement, begin, end);
	if (!expr)
		return NULL;

	Ral_Object* expr_result = Ral_EvaluateExpression(state, local_variables, expr);
	Ral_DestroyExpression(expr);
	return expr_result;
}



Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const local_variables,
	Ral_Object** return_object
)
{
	// The number of tokens in a statement should always be more than 1 so this should be safe
	// If this crashes then something with the lexer has created an empty, useless statement
	RalCLI_DEBUGLOG("Executing statement on line %i", statement->tokens[0].linenum);
	Ral_PrintStatementTokens(statement);

	switch (statement->type)
	{

	case Ral_STATEMENTTYPE_DECLARATION:
	{
		Ral_Type* decl_type = Ral_GetType(state, statement->tokens[0].string);
		if (!decl_type)
		{
			RalCLI_ERROR("Missing type!");
			return NULL;
		}
		if (statement->numtokens <= 1)
		{
			RalCLI_ERROR("Missing variable name for declaration!");
			return NULL;
		}
		Ral_Object* var = Ral_DeclareVariable(
			local_variables,
			statement->tokens[1].string,
			decl_type
		);
		Ral_Object* result = build_and_eval_expression(state, local_variables, statement, 1, statement->numtokens);
		Ral_DestroyObject(result);
	}
		break;

	case Ral_STATEMENTTYPE_EXPRESSION:
	{
		Ral_Object* result = build_and_eval_expression(state, local_variables, statement, 0, statement->numtokens);
		Ral_DestroyObject(result);
	}
		break;

	case Ral_STATEMENTTYPE_RETURN:
	{
		Ral_Object* result = build_and_eval_expression(state, local_variables, statement, 1, statement->numtokens);
		*return_object = result;
		return NULL;
	}
		break;

	default:
		break;
	}
	
	if (statement->index + 1 >= ((Ral_SourceUnit*)statement->parentsource)->numstatements) return NULL; // At last statement
	return &(((Ral_SourceUnit*)statement->parentsource)->statements[statement->index + 1]);
}

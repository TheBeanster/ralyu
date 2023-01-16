#include "ral_execute.h"

#include "ral_logging.h"
#include "ral_expression.h"
#include "ral_lexer.h"



Ral_Object* Ral_ExecuteSourceUnit(
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



static Ral_Statement* find_next_statementtype_in_scope(
	const Ral_Statement* const start,
	const Ral_StatementType type
)
{
	int scopedepth = 0;
	Ral_SourceUnit* source = start->parentsource;
	for (int i = start->index + 1; i < source->numstatements; i++)
	{
		Ral_Statement* s = &source->statements[i];

		if (s->type == type) return s;

		switch (s->type)
		{
		case Ral_STATEMENTTYPE_IF:
		case Ral_STATEMENTTYPE_FOR:
		case Ral_STATEMENTTYPE_WHILE:
			scopedepth++;
			break;

		case Ral_STATEMENTTYPE_END:
			scopedepth--;
			if (scopedepth < 0) return NULL;
			break;

		default:
			break;
		}
	}
}




#define GO_NEXT_STATEMENT(statement) \
	if (statement->index + 1 >= statement->parentsource->numstatements) return NULL; \
	return &statement->parentsource->statements[statement->index + 1];

Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const scope_variables,
	const Ral_Bool global,
	Ral_Object** const return_object
)
{
	printf(" - Executing statement - : ");
	Ral_PrintStatementString(statement);
	putchar('\n');

	switch (statement->type)
	{



	case Ral_STATEMENTTYPE_EXPRESSION:
	{
		
	}
		break;



	case Ral_STATEMENTTYPE_IF:
	{
		Ral_Object* expr_result = Ral_EvaluateExpression(state, statement->tokens, 1, statement->numtokens - 1);
		
		if (Ral_ObjectIsTrue(expr_result))
		{
			// Expression was true
			break;
		} else
		{
			// Expression was false
			// Search for 'else'
			Ral_Statement* elsestatement = find_next_statementtype_in_scope(statement, Ral_STATEMENTTYPE_ELSE);
			if (!elsestatement)
			{
				// No 'else' was found
				// Jump to 'end'
				Ral_Statement* endstatement = find_next_statementtype_in_scope(statement, Ral_STATEMENTTYPE_END);
				if (!endstatement)
				{
					// No end statement for if
					state->errormsg = "No 'end' found for if statement.";
					return NULL;
				}

				GO_NEXT_STATEMENT(endstatement);
			} else
			{
				// Else was found
				// Jump to else
				GO_NEXT_STATEMENT(elsestatement);
			}
		}
	}
		break;



	case Ral_STATEMENTTYPE_ELSE:
	{

	}
		break;



	case Ral_STATEMENTTYPE_FUNCTION:
	{
		Ral_Token* funcname = &statement->tokens[1];
		if (!funcname) return NULL;

		
	}
		break;



	default:
		printf("Invalid statement type\n");
		return NULL;
		break;
	}

	GO_NEXT_STATEMENT(statement);
}

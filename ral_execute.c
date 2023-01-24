#include "ral_execute.h"

#include "ralu_memory.h"

#include "ral_logging.h"
#include "ral_expression.h"
#include "ral_lexer.h"
#include "ral_function.h"



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
	Ral_List stack = { 0 }; // Stack of statements?

	while (current_statement)
	{
		current_statement = Ral_ExecuteStatement(
			state,
			current_statement,
			&stack,
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

	return NULL;
}




#define GO_NEXT_STATEMENT(statement) \
	if (statement->index + 1 >= statement->parentsource->numstatements) return NULL; \
	return &statement->parentsource->statements[statement->index + 1];

Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const stack,
	Ral_List* const scope_variables,
	const Ral_Bool global,
	Ral_Object** const return_object
)
{
#ifdef Ral_USE_DEBUG_PRINT
	printf(" - Executing statement - : ");
	Ral_PrintStatementString(statement);
	putchar('\n');
#endif

	switch (statement->type)
	{



	case Ral_STATEMENTTYPE_EXPRESSION:
	{
		Ral_Object* expr_result = Ral_EvaluateExpression(state, scope_variables, statement->tokens, 0, statement->numtokens);
		Ral_DestroyObject(expr_result);
	}
		break;



	case Ral_STATEMENTTYPE_RETURN:
	{
		*return_object = Ral_EvaluateExpression(state, scope_variables, statement->tokens, 1, statement->numtokens);
		return NULL;
	}
		break;



	case Ral_STATEMENTTYPE_IF:
	{
		Ral_Object* expr_result = Ral_EvaluateExpression(state, scope_variables, statement->tokens, 1, statement->numtokens - 1);
		
		if (Ral_ObjectIsTrue(expr_result))
		{
			// Expression was true
			Ral_PushBackList(stack, statement);
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
				Ral_PushBackList(stack, elsestatement);
				GO_NEXT_STATEMENT(elsestatement);
			}
		}
	}
		break;



	case Ral_STATEMENTTYPE_ELSE:
	{
		Ral_Statement* topstatement = Ral_PopBackList(stack);
		if (topstatement->type != Ral_STATEMENTTYPE_IF)
		{
			state->errormsg = "Else found without if statement";
			return NULL;
		}

		Ral_Statement* endstatement = find_next_statementtype_in_scope(statement, Ral_STATEMENTTYPE_END);
		if (!endstatement)
		{
			state->errormsg = "No 'end' found for if statement";
			return NULL;
		}
		GO_NEXT_STATEMENT(endstatement);
	}
		break;



	case Ral_STATEMENTTYPE_END:
	{
		Ral_Statement* topstatement = Ral_PopBackList(stack);

		if (!topstatement)
		{
			return NULL;
		}

		switch (topstatement->type)
		{
		case Ral_STATEMENTTYPE_WHILE:
			return topstatement;
			break;

		case Ral_STATEMENTTYPE_IF:
			break;

		default:
			printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA NOT IMPLEMENTED END!!\n");
			break;
		}
	}
		break;



	case Ral_STATEMENTTYPE_WHILE:
	{
		Ral_Object* expr_result = Ral_EvaluateExpression(state, scope_variables, statement->tokens, 1, statement->numtokens);
		if (Ral_ObjectIsTrue(expr_result))
		{
			// Expression was true
			Ral_PushBackList(stack, statement);
			// ENTER DA LOOP
			break;
		} else
		{
			// Jump to 'end'
			Ral_Statement* endstatement = find_next_statementtype_in_scope(statement, Ral_STATEMENTTYPE_END);
			if (!endstatement)
			{
				// No end statement for if
				state->errormsg = "No 'end' found for if statement.";
				return NULL;
			}

			GO_NEXT_STATEMENT(endstatement);
		}
	}
		break;



	case Ral_STATEMENTTYPE_FUNCTION:
	{
		Ral_SourceUnit* source = statement->parentsource;

		if (statement->numtokens <= 1) return NULL; // No function name
		Ral_Token* funcname = &statement->tokens[1];
		if (!funcname) return NULL;

		// Make sure there is a parenthesis
		if (statement->numtokens <= 2) return NULL; // No token
		if (statement->tokens[2].separatorid != Ral_SEPARATOR_LPAREN) return NULL; // Not a parenthesis

		// Find start and end of function body
		if (statement->index + 1 >= source->numstatements) return NULL; // No next statement
		Ral_Statement* endstatement = find_next_statementtype_in_scope(statement, Ral_STATEMENTTYPE_END);
		if (!endstatement) return NULL; // No end for function

		Ral_Statement* bodystart = NULL;
		Ral_Statement* bodyend = NULL;
		if (endstatement->index == statement->index + 1)
		{
			// Function has no body
		} else
		{
			bodystart = &source->statements[statement->index + 1];
			bodyend = endstatement;
		}
		
		// Read arguments
		if (statement->numtokens <= 3) return NULL; // No closing parenthesis
		Ral_List arguments = { 0 };
		Ral_Bool* expect_identifier = Ral_TRUE;
		for (int i = 3; i < statement->numtokens; i++)
		{
			Ral_Token* tok = &statement->tokens[i];

			if (expect_identifier && (tok->type == Ral_TOKENTYPE_IDENTIFIER))
			{
				Ral_FunctionArgument* arg = Ral_ALLOC_TYPE(Ral_FunctionArgument);
				arg->name = _strdup(tok->string);
				Ral_PushBackList(&arguments, arg);
				expect_identifier = Ral_FALSE;
			} else if ((!expect_identifier) && (tok->separatorid == Ral_SEPARATOR_COMMA))
			{
				expect_identifier = Ral_TRUE;
			}
			else if (
				((tok->separatorid == Ral_SEPARATOR_RPAREN) && (!expect_identifier)) || 
				((tok->separatorid == Ral_SEPARATOR_RPAREN) && (i == 3)))
				break;
			else
			{
				// Invalid token
				printf("AAAA INVALID TOKEN!!!\n");
				// Free the arguments list
				Ral_FunctionArgument* iter = arguments.begin;
				while (iter)
				{
					Ral_FunctionArgument* del = iter;
					iter = iter->next;
					Ral_FREE(del->name);
					Ral_FREE(del);
				}
				return NULL;
			}
		}

		

		Ral_Function* func = Ral_CreateFunction(
			bodystart,
			bodyend,
			funcname->string,
			&arguments
		);
		Ral_DeclareFunction(state, func);

		// Free the arguments list
		Ral_FunctionArgument* iter = arguments.begin;
		while (iter)
		{
			Ral_FunctionArgument* del = iter;
			iter = iter->next;
			Ral_FREE(del->name);
			Ral_FREE(del);
		}

		GO_NEXT_STATEMENT(endstatement);
	}
		break;



	default:
		printf("Invalid statement type\n");
		return NULL;
		break;
	}

	GO_NEXT_STATEMENT(statement);
}

#include "ral_expression.h"

#include <stdio.h>
#include "ralu_string.h"
#include "ralu_list.h"
#include "ralu_memory.h"

#include "ral_lexer.h"
#include "ral_function.h"
#include "ral_variable.h"



static Ral_Object* get_token_value(
	Ral_State* const state,
	const Ral_List* const local_variables,
	Ral_Token* const token
)
{
	if (!token) return NULL;

	if (token->expr_value)
	{
		Ral_Object* obj = token->expr_value;
		token->expr_value = NULL;
		return obj;
	}
	if (token->type == Ral_TOKENTYPE_INTLITERAL)
	{
		return Ral_CreateNumberObject(atoi(token->string));
	} else if (token->type == Ral_TOKENTYPE_FLOATLITERAL)
	{
		return Ral_CreateNumberObject(atof(token->string));
	} else if (token->type == Ral_TOKENTYPE_KEYWORD)
	{
		if (token->keywordid == Ral_KEYWORD_TRUE)
			return Ral_CreateBoolObject(Ral_TRUE);
		else if (token->keywordid == Ral_KEYWORD_FALSE)
			return Ral_CreateBoolObject(Ral_FALSE);
	} else if (token->type == Ral_TOKENTYPE_IDENTIFIER)
	{
		Ral_Variable* var = Ral_GetOrDeclareVariable(state, local_variables, token->string);
		if (!var)
		{
			state->errormsg = "Missing variable";
			return NULL;
		}
		return var->obj;
	}
	return NULL;
}



static int get_next_unnested_separator(
	const Ral_Token* const tokens,
	const Ral_SeparatorID separator,
	const int begin,
	const int end
)
{
	int depth = 0;
	for (int i = begin + 1; i < end; i++)
	{
		Ral_Token* tok = &tokens[i];
		if ((depth == 0) && (tok->separatorid == separator)) return i;
		if (depth < 0) return -1;

		switch (tok->separatorid)
		{
		case Ral_SEPARATOR_LPAREN:
		case Ral_SEPARATOR_LBRACE:
		case Ral_SEPARATOR_LBRACKET:
			depth++;
			break;
		case Ral_SEPARATOR_RPAREN:
		case Ral_SEPARATOR_RBRACE:
		case Ral_SEPARATOR_RBRACKET:
			depth--;
			break;
		default:
			break;
		}
	}
	return -1;
}



typedef struct expr_list_elem
{
	Ral_LISTLINKS(expr_list_elem);

	Ral_Token* token;
	int precedence;

} expr_list_elem;

Ral_Object* Ral_EvaluateExpression(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_Token* const tokens,
	const int begin,
	const int end
)
{
	DEBUG_PRINT("Evaluating expression\n");

#ifdef Ral_USE_DEBUG_PRINT
	// Print all tokens in the expression
	for (int i = begin; i < end; i++)
	{
		printf("%s ", tokens[i].string);
	}
	putchar('\n');
#endif

	Ral_Object* final_result = NULL;

	int numtokens = end - begin;
	if (numtokens <= 0)
	{
		printf("Expression is missing what?\n");
		return NULL;
	}
	else if (numtokens == 1)
	{
		// Expression is only one token
		return get_token_value(state, local_variables, &tokens[begin]);
	}

	// Get the operators and arguments
	Ral_List l_tokens = { 0 };
	Ral_List l_operators = { 0 };
	int current_nesting = 0;
	for (int i = begin; i < end; i++)
	{
		Ral_Token* tok = &tokens[i];

		if (tok->type == Ral_TOKENTYPE_OPERATOR)
		{
			expr_list_elem* operator = Ral_ALLOC_TYPE(expr_list_elem);
			operator->token = tok;
			operator->precedence = (current_nesting << Ral_EXPRESSION_NESTING_PRECEDENCE_SHIFT) + ral_operator_precedence[tok->operatorid];
			Ral_PushBackList(&l_operators, operator);

			// Insert the token in the l_tokens list
			tok->prev = NULL;
			tok->next = NULL;
			Ral_PushBackList(&l_tokens, tok);

		}
		else if (tok->type == Ral_TOKENTYPE_IDENTIFIER)
		{
			// Get the token after this token
			Ral_Token* nexttoken = NULL;
			if(i + 1 < end)
				nexttoken = &tokens[i + 1];

			if (nexttoken && nexttoken->separatorid == Ral_SEPARATOR_LPAREN)
			{
				// If the token after an identifier is a left parenthesis then it is a function call
				Ral_Function* func = Ral_GetFunction(state, tok->string);
				if (!func)
				{
					printf("Function \"%s\" doesn't exist\n", tok->string);
					goto free_and_exit;
				}

				Ral_List args = { 0 };
				for (i = i + 1; i < end;) // i starts at (
				{
					// The index of the token of the end of the argument
					int arg_end = get_next_unnested_separator(tokens, Ral_SEPARATOR_COMMA, i, end);
					if (arg_end < 0)
					{
						// If no comma was found then get the end parenthesis
						arg_end = get_next_unnested_separator(tokens, Ral_SEPARATOR_RPAREN, i, end);
					}
					
					if (i + 1 < arg_end)
					{
						// Evaluate everything from j to the arg_end
						Ral_Object* arg = Ral_EvaluateExpression(state, local_variables, tokens, i + 1, arg_end);
						if (arg)
							Ral_PushBackList(&args, arg);
					}

					i = arg_end;
					if (tokens[arg_end].separatorid != Ral_SEPARATOR_COMMA) break;
				}
				
			#ifdef Ral_USE_DEBUG_PRINT
				Ral_Object* iter = args.begin;
				while (iter)
				{
					Ral_PrintObjectValue(iter);
					putchar('\n');
					iter = iter->next;
				}
			#endif

				Ral_Object* call_result = Ral_CallFunction(state, func, &args);

				tok->prev = NULL;
				tok->next = NULL;
				tok->expr_value = call_result;
				Ral_PushBackList(&l_tokens, tok);
			} else
			{
				// Identifier should be seen as variable
				Ral_Variable* var = Ral_GetOrDeclareVariable(state, local_variables, tok->string);
				if (!var)
				{
					goto free_and_exit;
				}
				tok->expr_value = var->obj;
				Ral_PushBackList(&l_tokens, tok);
			}
		}
		else if (
			tok->type == Ral_TOKENTYPE_INTLITERAL ||
			tok->type == Ral_TOKENTYPE_FLOATLITERAL ||
			tok->type == Ral_TOKENTYPE_STRINGLITERAL ||
			tok->type == Ral_TOKENTYPE_KEYWORD)
		{
			// Insert the token in the l_tokens list
			tok->prev = NULL;
			tok->next = NULL;
			Ral_PushBackList(&l_tokens, tok);
			tok->expr_value = get_token_value(state, local_variables, tok);
		}
		else if (tok->separatorid == Ral_SEPARATOR_LPAREN)
		{
			current_nesting++;
		}
		else if (tok->separatorid == Ral_SEPARATOR_RPAREN)
		{
			current_nesting--;
			if (current_nesting < 0)
			{
				printf("Too many right parentheses\n");
				goto free_and_exit;
			}
		}
		else
		{
			printf("Invalid expression token\n");
			goto free_and_exit;
		}

	}

	if (l_operators.itemcount == 0)
	{
		if (l_tokens.begin)
			final_result = ((Ral_Token*)l_tokens.begin)->expr_value;
		goto free_and_exit;
	}

	// Sort the l_operators list
	// Bubble sort courtesy of j-javapoint? wtf java f u
	expr_list_elem* current;
	expr_list_elem* index;
	int temp;
	for (current = l_operators.begin; current->next != NULL; current = current->next)
	{
		for (index = current->next; index != NULL; index = index->next)
		{
			if (current->precedence < index->precedence)
			{
				Ral_Token* temp_token = current->token;
				int temp_precedence = current->precedence;

				current->token = index->token;
				current->precedence = index->precedence;

				index->token = temp_token;
				index->precedence = temp_precedence;
			}
		}
	}
	// TODO There might be some issues with the sorting



	expr_list_elem* iterator = l_operators.begin;
	while (1)
	{
		Ral_OperatorID op = iterator->token->operatorid;

		if (Ral_IS_UNARY_OPERATOR(op))
		{
			Ral_UnlinkFromList(&l_operators, iterator);
		} else if (Ral_IS_ARITHEMTIC_OPERATOR(op))
		{
			Ral_Object* left	= get_token_value(state, local_variables, iterator->token->prev);
			Ral_Object* right	= get_token_value(state, local_variables, iterator->token->next);
			if ((left && right) == 0)
			{
				state->errormsg == "Null operand";
				goto free_and_exit;
			}

			iterator->token->expr_value = Ral_ObjectArithmeticOperator(state, op, left, right);

			Ral_UnlinkFromList(&l_tokens, iterator->token->prev);
			Ral_UnlinkFromList(&l_tokens, iterator->token->next);
		} else if (Ral_IS_ASSIGNMENT_OPERATOR(op))
		{
			if (iterator->token->prev->type != Ral_TOKENTYPE_IDENTIFIER)
			{
				state->errormsg = "Cannot assign to non-variable";
				goto free_and_exit;
			}
			Ral_Variable* var = Ral_GetOrDeclareVariable(state, local_variables, iterator->token->prev->string);
			iterator->token->expr_value = Ral_ObjectAssigmentOperator(
				state,
				op,
				&var->obj,
				get_token_value(state, local_variables, iterator->token->next)
			);
			Ral_UnlinkFromList(&l_tokens, iterator->token->prev);
			Ral_UnlinkFromList(&l_tokens, iterator->token->next);
		} else if (Ral_IS_RELATIONAL_OPERATOR(op))
		{
			Ral_Object* left	= get_token_value(state, local_variables, iterator->token->prev);
			Ral_Object* right	= get_token_value(state, local_variables, iterator->token->next);

			iterator->token->expr_value = Ral_CreateBoolObject(Ral_ObjectRelationalOperator(state, op, left, right));

			Ral_UnlinkFromList(&l_tokens, iterator->token->prev);
			Ral_UnlinkFromList(&l_tokens, iterator->token->next);
		}

		if (!iterator->next)
		{
			// End of list
			final_result = iterator->token->expr_value;
			break;
		} else
			iterator = iterator->next;
	}



free_and_exit:

	iterator = l_operators.begin;
	while (iterator)
	{
		expr_list_elem* del = iterator;
		iterator = iterator->next;
		Ral_FREE(del);
	}

	Ral_Token* tokiter = l_tokens.begin;
	while (tokiter)
	{
		Ral_Object* del = tokiter->expr_value;
		if (del != final_result)
		{
			tokiter->expr_value = NULL;
			Ral_DestroyObject(del);
		}
		tokiter = tokiter->next;
	}
	
#ifdef Ral_USE_DEBUG_PRINT
	printf("Expression result = ");
	Ral_PrintObjectValue(final_result);
	putchar('\n');
#endif

	return final_result;
}

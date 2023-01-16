#include "ral_expression.h"

#include <stdio.h>
#include "ralu_string.h"
#include "ralu_list.h"
#include "ralu_memory.h"

#include "ral_lexer.h"



Ral_Object* get_token_value(
	const Ral_State* const state,
	const Ral_Token* const token
)
{
	if (token->type == Ral_TOKENTYPE_IDENTIFIER)
	{
		return Ral_CreateNumberObject(0);
	} else if (token->type == Ral_TOKENTYPE_INTLITERAL)
	{
		return Ral_CreateNumberObject(atoi(token->string));
	} else if (token->type == Ral_TOKENTYPE_FLOATLITERAL)
	{
		return Ral_CreateNumberObject(atof(token->string));
	}
}



typedef struct expr_list_elem
{
	Ral_LISTLINKS(expr_list_elem);

	Ral_Token* token;
	int precedence;

} expr_list_elem;

Ral_Object* Ral_EvaluateExpression(
	Ral_State* const state,
	const Ral_Token* const tokens,
	const int begin,
	const int end
)
{
	printf("Evaluating expression\n");

	// Print all tokens in the expression
	for (int i = begin; i < end; i++)
	{
		printf("%s ", tokens[i].string);
	}
	putchar('\n');

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
		return get_token_value(state, &tokens[begin]);
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
		else if (
			tok->type == Ral_TOKENTYPE_IDENTIFIER || 
			tok->type == Ral_TOKENTYPE_INTLITERAL ||
			tok->type == Ral_TOKENTYPE_FLOATLITERAL ||
			tok->type == Ral_TOKENTYPE_STRINGLITERAL)
		{
			// Insert the token in the l_tokens list
			tok->prev = NULL;
			tok->next = NULL;
			Ral_PushBackList(&l_tokens, tok);
			tok->expr_value = get_token_value(state, tok);
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
		} else
		{
			// Operator is binary
			Ral_Object* left = get_token_value(state, iterator->token->prev);
			Ral_Object* right = get_token_value(state, iterator->token->next);

			switch (op)
			{
			case Ral_OPERATOR_ADDITION: iterator->token->expr_value = Ral_ObjectAdd(left, right); break;

			case Ral_OPERATOR_EQUALITY: iterator->token->expr_value = Ral_ObjectEqual(left, right); break;
			default:
				break;
			}

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
	
	printf("Expression result = ");
	Ral_PrintObjectValue(final_result);
	putchar('\n');

	return final_result;
}

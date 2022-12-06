#include "ral_expression.h"

#include "ralu_memory.h"

#include "ral_cli.h"
#include "ral_lexer.h"



typedef struct expr_list_elem
{
	Ral_LISTLINKS(expr_list_elem);
	Ral_Token* token;
	int precedence;
} expr_list_elem;

Ral_Expression* Ral_CreateExpression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
)
{
	// Validate
	if (!statement) return NULL;
	if (begin < 0) return NULL;
	if (end <= begin) return NULL;
	if (end > statement->numtokens) return NULL;

	Ral_Expression* expression = Ral_ALLOC_TYPE(Ral_Expression);

	Ral_List l_tokens = { 0 };
	Ral_List l_operators = { 0 };
	int current_paren_depth = 0;
	for (int i = begin; i < end; i++)
	{
		Ral_Token* token = &statement->tokens[i];
		
		if (token->type == Ral_TOKENTYPE_OPERATOR)
		{
			expr_list_elem* operator = Ral_ALLOC_TYPE(expr_list_elem);
			operator->token = token;
			operator->precedence = 
				(current_paren_depth << Ral_EXPRESSION_NESTING_PRECEDENCE_SHIFT) + 
				ral_operator_precedence[token->operatorid];
			Ral_PushFrontList(&l_operators, operator);

			// Insert the token in the l_tokens list
			token->prev = NULL;
			token->next = NULL;
			Ral_PushFrontList(&l_tokens, token);

		}
		else if (token->type == Ral_TOKENTYPE_IDENTIFIER || token->type == Ral_TOKENTYPE_NUMBERLITERAL)
		{
			// Insert the token in the l_tokens list
			token->prev = NULL;
			token->next = NULL;
			Ral_PushFrontList(&l_tokens, token);
		}
		else if (token->separatorid == Ral_SEPARATOR_LPAREN)
		{
			current_paren_depth++;
		}
		else if (token->separatorid == Ral_SEPARATOR_RPAREN)
		{
			current_paren_depth--;
			if (current_paren_depth < 0)
			{
				RalCLI_ERROR("Too many right parentheses\n");
				goto free_and_exit;
			}
		}
		else
		{
			RalCLI_ERROR("Invalid expression token\n");
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
	while (iterator)
	{
		

		iterator = iterator->next;
	}

	return expression;

free_and_exit:
	return NULL;
}





void Ral_DestroyExpression(
	Ral_Expression* const expression
)
{
	// TODO Implement
}

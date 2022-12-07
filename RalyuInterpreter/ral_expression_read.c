#include "ral_expression.h"

#include "ralu_memory.h"
#include <assert.h>

#include "ral_cli.h"
#include "ral_lexer.h"



typedef struct expr_list_elem
{
	Ral_LISTLINKS(expr_list_elem);
	Ral_Token* token;
	int precedence;
} expr_list_elem;

static Ral_ExprNode* read_expression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
)
{
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
		else if (
			token->type == Ral_TOKENTYPE_IDENTIFIER ||
			token->type == Ral_TOKENTYPE_NUMBERLITERAL ||
			token->type == Ral_TOKENTYPE_STRINGLITERAL)
		{
			// Insert the token in the l_tokens list
			token->prev = NULL;
			token->next = NULL;
			Ral_PushFrontList(&l_tokens, token);

			// TODO Check if it's a function call
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
		// Loop over all operators from highest to lowest precedence

		Ral_OperatorID op = iterator->token->operatorid;

		printf("op = %s, l = %s, r = %s\n",
			ral_operatorid_names[op],
			iterator->token->prev->string,
			iterator->token->next->string);

		if (Ral_IS_UNARY_OPERATOR(op))
		{
			// Operator is unary
			if (op == Ral_OPERATOR_NEGATIVE)
			{
				// Make sure there is a token to the right of the negative sign
				if (!iterator->token->next) goto free_and_exit;
				
				// Create the operator node
				Ral_ExprNode* node = Ral_ALLOC_TYPE(Ral_ExprNode);
				node->parent = NULL;
				node->left = NULL;
				node->right = NULL;
				node->corresp_token = iterator->token;
				node->type = Ral_EXPRNODETYPE_OPERATOR;

				Ral_ExprNode* right_node = iterator->token->next->expr_node;
				if (!right_node)
				{
					// Tree node doens't already exist
					right_node = Ral_ALLOC_TYPE(Ral_ExprNode);
					right_node->parent = node;
					right_node->left = NULL;
					right_node->right = NULL;
					right_node->corresp_token = iterator->token->next;
					right_node->type = 0;												//!!! TODO GET TYPE
					Ral_UnlinkFromList(&l_tokens, iterator->token->next);
				} else
				{
					// Token is already in the tree
					assert(right_node->parent == NULL);
					// Just set it's tree links
					right_node->parent = node;
					Ral_UnlinkFromList(&l_tokens, iterator->token->next);
				}
				node->right = right_node;
				
			} else
			{
				// What other operator is unary?
				RalCLI_ERROR("What other operator is unary?\n");
			}
		} else
		{
			// Operator is binary
			// Make sure there is a token to the left and right of the negative sign
			if (!iterator->token->prev || !iterator->token->next) goto free_and_exit;

			// Create the operator node
			Ral_ExprNode* node = Ral_ALLOC_TYPE(Ral_ExprNode);
			node->parent = NULL;
			node->left = NULL;
			node->right = NULL;
			node->corresp_token = iterator->token;
			node->type = Ral_EXPRNODETYPE_OPERATOR;

			Ral_ExprNode* left_node = iterator->token->prev->expr_node;
			if (!left_node)
			{
				// Tree node doens't already exist
				left_node = Ral_ALLOC_TYPE(Ral_ExprNode);
				left_node->parent = node;
				left_node->left = NULL;
				left_node->right = NULL;
				left_node->corresp_token = iterator->token->prev;
				left_node->type = 0;
				Ral_UnlinkFromList(&l_tokens, iterator->token->prev);
			} else
			{
				// Token is already in the tree
				assert(left_node->parent == NULL);
				// Just set it's tree links and remove from the tokens list
				left_node->parent = node;
				Ral_UnlinkFromList(&l_tokens, iterator->token->prev);
			}
			node->left = left_node;

			Ral_ExprNode* right_node = iterator->token->next->expr_node;
			if (!right_node)
			{
				// Tree node doens't already exist
				right_node = Ral_ALLOC_TYPE(Ral_ExprNode);
				right_node->parent = node;
				right_node->left = NULL;
				right_node->right = NULL;
				right_node->corresp_token = iterator->token->next;
				right_node->type = 0;
				Ral_UnlinkFromList(&l_tokens, iterator->token->next);
			} else
			{
				// Token is already in the tree
				assert(right_node->parent == NULL);
				// Just set it's tree links
				right_node->parent = node;
				Ral_UnlinkFromList(&l_tokens, iterator->token->next);
			}
			node->right = right_node;
		}

		iterator = iterator->next;
	}

	return iterator;

free_and_exit:
	RalCLI_ERROR("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
	exit(-1000000);
	return NULL;
}





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

	expression->top = read_expression(statement, begin, end);

	return expression;
}





void Ral_DestroyExpression(
	Ral_Expression* const expression
)
{
	// TODO Implement
}

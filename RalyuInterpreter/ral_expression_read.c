#include "ral_expression.h"

#include "ralu_memory.h"
#include <assert.h>

#include "ral_cli.h"
#include "ral_lexer.h"



static Ral_ExprNodeType get_type(const Ral_Token* const token)
{
	if (token->type == Ral_TOKENTYPE_STRINGLITERAL ||
		token->type == Ral_TOKENTYPE_NUMBERLITERAL)
		return Ral_EXPRNODETYPE_LITERAL;
	if (token->type == Ral_TOKENTYPE_IDENTIFIER)
	{
		// If the token has an expression node and that node has functioncall parameters then it is prob a function call
		if (token->expr_node && token->expr_node->functioncall_parameters.itemcount > 0)
			return Ral_EXPRNODETYPE_FUNCTION;
		else
			return Ral_EXPRNODETYPE_VARIABLE;
	}
}

static Ral_ExprNode* read_expression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
);



/// @brief Takes a starting token in a statement, reads a function call, puts the expr_node of that function call in the token
/// and returns the position where the function call ends.
/// @param statement The statement the function call is inside.
/// @param begin The index of the token that is the function call.
/// @param token A pointer to that token.
/// @return The index in the tokens array of the statement where the function ends.
static int read_function_call(
	const Ral_Statement* const statement,
	const int begin,
	Ral_Token* const token
)
{
	// Make sure the token doesn't already have parameters
	if (token->expr_node &&
		(token->expr_node->functioncall_parameters.itemcount > 0)) return -1;

	// The exprnode of the function call
	Ral_ExprNode* node = Ral_ALLOC_TYPE(Ral_ExprNode);
	node->corresp_token = token;
	token->expr_node = node;
	node->type = Ral_EXPRNODETYPE_FUNCTION;

	int paren_depth = 0; // The depth level of parentheses
	int param_begin = begin + 2; // Where the current parameter being read started
	// begin + 2 is the token after the opening parenthesis

	for (int i = begin + 2; i < statement->numtokens; i++)
	{
		Ral_Token* curtoken = &statement->tokens[i];

		if (paren_depth == 0 && curtoken->separatorid == Ral_SEPARATOR_COMMA)
		{
			Ral_ExprNode* param_node = NULL;
			if (!(i <= param_begin || i > statement->numtokens))
				param_node = read_expression(statement, param_begin, i);
			if (param_node)
				Ral_PushFrontList(&node->functioncall_parameters, param_node);
			else
				goto free_and_exit;
			param_begin = i + 1;
		}
		
		if (curtoken->separatorid == Ral_SEPARATOR_LPAREN)
		{
			paren_depth++;
		}
		if (curtoken->separatorid == Ral_SEPARATOR_RPAREN)
		{
			paren_depth--;
			if (paren_depth < 0)
			{
				Ral_ExprNode* param_node = NULL;
				if (!(i <= param_begin || i > statement->numtokens))
					param_node = read_expression(statement, param_begin, i);
				if (param_node)
					Ral_PushFrontList(&node->functioncall_parameters, param_node);
				else
					goto free_and_exit;
				param_begin = i + 1;
				break;
			}
		}
	}

	return param_begin - 1;

free_and_exit:
	Ral_DestroyExprNode(node);
	token->expr_node = NULL;
}



typedef struct expr_operator
{
	Ral_LISTLINKS(expr_operator);
	Ral_Token* token;
	int precedence;
} expr_operator;

static Ral_ExprNode* read_expression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
)
{
	/*
	The first step of building the expression tree is finding all operators and operands.
	They will be put in the l_tokens and l_operators lists. The l_tokens has every operand
	and operator and uses the list links of the tokens themselves. The l_operators has
	expr_list_elem and has all the operators and also what precedence they have. 

	After they've been read the l_operators list will be sorted so the operator with highest
	precedence comes first. And at last, the actual tree structure will be created by looping
	over the operators and getting the operands from the l_tokens list.
	*/

	if (end - 1 <= begin)
	{
		// Expression is one token long
		Ral_ExprNode* node = Ral_ALLOC_TYPE(Ral_ExprNode);
		node->corresp_token = &statement->tokens[begin];
		node->type = get_type(node->corresp_token);
		return node;
	}

	Ral_ExprNode* topnode = NULL;
	Ral_List l_tokens = { 0 };
	Ral_List l_operators = { 0 };
	int current_paren_depth = 0;
	for (int i = begin; i < end; i++)
	{
		Ral_Token* token = &statement->tokens[i];

		if (token->type == Ral_TOKENTYPE_OPERATOR)
		{
			expr_operator* operator = Ral_ALLOC_TYPE(expr_operator);
			operator->token = token;
			operator->precedence = 
				(current_paren_depth << Ral_EXPRESSION_NESTING_PRECEDENCE_SHIFT) + 
				ral_operator_precedence[token->operatorid];
			Ral_PushFrontList(&l_operators, operator);

			// Insert the token in the l_tokens list
			token->prev = NULL;
			token->next = NULL;
			Ral_PushFrontList(&l_tokens, token);

			token->expr_node = Ral_ALLOC_TYPE(Ral_ExprNode);
			token->expr_node->corresp_token = token;
			token->expr_node->type = Ral_EXPRNODETYPE_OPERATOR;
		}
		else if (
			token->type == Ral_TOKENTYPE_NUMBERLITERAL ||
			token->type == Ral_TOKENTYPE_STRINGLITERAL)
		{
			token->prev = NULL;
			token->next = NULL;
			Ral_PushFrontList(&l_tokens, token);

			token->expr_node = Ral_ALLOC_TYPE(Ral_ExprNode);
			token->expr_node->corresp_token = token;
			token->expr_node->type = Ral_EXPRNODETYPE_LITERAL;
		}
		else if (token->type == Ral_TOKENTYPE_IDENTIFIER)
		{
			// Check if identifier is a function call
			if ((i + 1 < statement->numtokens) &&
				(statement->tokens[i + 1].separatorid == Ral_SEPARATOR_LPAREN))
			{
				// Identifier is function call
				// Eval the func
				i = read_function_call(
					statement,
					i,
					token
				);
				token->prev = NULL;
				token->next = NULL;
				Ral_PushFrontList(&l_tokens, token);
			} else
			{
				token->prev = NULL;
				token->next = NULL;
				Ral_PushFrontList(&l_tokens, token);

				token->expr_node = Ral_ALLOC_TYPE(Ral_ExprNode);
				token->expr_node->corresp_token = token;
				token->expr_node->type = Ral_EXPRNODETYPE_VARIABLE;
			}
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

	// Check if the operators list is empty
	if (l_operators.itemcount == 0)
	{
		// If so return the first token
		return ((Ral_Token*)l_tokens.begin)->expr_node;
	}

	// Sort the l_operators list
	expr_operator* current;
	expr_operator* index;
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

	expr_operator* iterator = l_operators.begin;
	while (iterator)
	{
		// Loop over all operators from highest to lowest precedence

		Ral_OperatorID op = iterator->token->operatorid;

		Ral_ExprNode* node = iterator->token->expr_node;

		printf("op = %s\n", ral_operatorid_names[op]);

		if (Ral_IS_UNARY_OPERATOR(op))
		{
			// Operator is unary
			if (op == Ral_OPERATOR_NEGATIVE)
			{
				// Make sure there is a token to the right of the negative sign
				if (!iterator->token->next) goto free_and_exit;
				
				Ral_ExprNode* right_node = iterator->token->next->expr_node;
				if (!right_node)
				{
					RalCLI_ERROR("FUCK");
					goto free_and_exit;
				} else
				{
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


			Ral_ExprNode* left_node = iterator->token->prev->expr_node;
			if (!left_node)
			{
				// Tree node doens't already exist
				RalCLI_ERROR("FUCKS");
				goto free_and_exit;
			} else
			{
				// Just set it's tree links and remove from the tokens list
				left_node->parent = node;
				Ral_UnlinkFromList(&l_tokens, iterator->token->prev);
			}
			node->left = left_node;

			Ral_ExprNode* right_node = iterator->token->next->expr_node;
			if (!right_node)
			{
				// Tree node doens't already exist
				RalCLI_ERROR("FUCKZ");
				goto free_and_exit;
			} else
			{
				// Just set it's tree links
				right_node->parent = node;
				Ral_UnlinkFromList(&l_tokens, iterator->token->next);
			}
			node->right = right_node;
		}

		if (!iterator->next)
		{
			topnode = node;
			break;
		}
		iterator = iterator->next;
	}



free_and_exit:
	Ral_ClearList(&l_operators, NULL);

	return topnode;
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








void Ral_DestroyExprNode(Ral_ExprNode* const node)
{
	if (!node) return;

	if (node->left)
		Ral_DestroyExprNode(node->left);
	if (node->right)
		Ral_DestroyExprNode(node->right);
	Ral_ExprNode* iterator = node->functioncall_parameters.begin;
	while (iterator)
	{
		Ral_DestroyExprNode(iterator);
		iterator = iterator->next;
	}

	Ral_FREE(node);
}



void Ral_DestroyExpression(
	Ral_Expression* const expression
)
{
	// TODO Implement
}

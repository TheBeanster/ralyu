#include "ral_expression.h"

#include "ral_function.h"

#include <stdio.h>



static Ral_Object* evaluate_binary_operator(
	const Ral_OperatorID operator,
	const Ral_ExprNode* const left,
	const Ral_ExprNode* const right
)
{

}



static Ral_Object* evaluate_node(Ral_ExprNode* const node)
{
	if (!node) return NULL;

	static int depth = 0;
	for (int i = 0; i < depth; i++)
		putchar(' ');
	printf("%s\n", node->corresp_token->string);
	depth++;

	switch (node->type)
	{
	case Ral_EXPRNODETYPE_OPERATOR: {}
		Ral_Token* token = node->corresp_token;
		if (!token) return NULL;
		if (token->operatorid == Ral_NOT_OPERATOR) return NULL;

		Ral_ExprNode* left_node = node->left;
		Ral_ExprNode* right_node = node->right;

		evaluate_node(left_node);
		evaluate_node(right_node);
		depth--;
		break;

	case Ral_EXPRNODETYPE_LITERAL:
		depth--;
		return Ral_CreateObjectFromLiteral(node->corresp_token);
		break;

	case Ral_EXPRNODETYPE_VARIABLE:
		depth--;
		return Ral_CreateIntObjectFromInt(0);
		break;

	case Ral_EXPRNODETYPE_FUNCTION:
		depth--;

		// Make a list of objects for the results for the parameters
		Ral_ExprNode* parameter_node = node->functioncall_parameters.begin;
		while (parameter_node)
		{
			depth += 2;
			evaluate_node(parameter_node);
			depth -= 2;
			parameter_node = parameter_node->next;
		}

		return Ral_CreateIntObjectFromInt(0);
		break;

	default:
		break;
	}
}



Ral_Object* Ral_EvaluateExpression(Ral_State* const state, const Ral_Expression* const expression)
{
	Ral_Object* result = evaluate_node(expression->top);

	return result;
}

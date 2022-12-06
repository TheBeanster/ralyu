#include "ral_expression.h"

#include "ral_function.h"



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

	switch (node->type)
	{
	case Ral_EXPRNODETYPE_OPERATOR: {}
		Ral_Token* token = node->corresp_token;
		if (!token) return NULL;
		if (token->operatorid == Ral_NOT_OPERATOR) return NULL;

		Ral_Object* left_arg = NULL;
		Ral_Object* right_arg = NULL;

		if (token->operatorid == Ral_OPERATOR_ASSIGN) // If operator is standard assign then don't get the left value
		{
			
			return;
		}

		left_arg = evaluate_node(node->left);
		if (!Ral_IS_UNARY_OPERATOR(token->operatorid)) // If operator is unary then don't get right value
		{
			// 
			right_arg = evaluate_node(node->right);
		} else
		{
			// Unary operator
		}

		break;

	case Ral_EXPRNODETYPE_LITERAL:
		return Ral_CreateObjectFromLiteral(node->corresp_token);
		break;

	case Ral_EXPRNODETYPE_FUNCTION:
		
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

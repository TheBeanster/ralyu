#include "ral_expression.h"



static Ral_Object* evaluate_node(Ral_ExprNode* const node)
{
	if (!node) return NULL;

	switch (node->type)
	{
	case Ral_EXPRNODETYPE_LITERAL:
		return NULL;
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

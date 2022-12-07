#include "ral_expression.h"

#include "ral_function.h"
#include "ral_cli.h"

#include <stdio.h>



static Ral_Object* evaluate_node(
	Ral_State* const state,
	Ral_List* const local_variables,
	Ral_ExprNode* const node
);



static Ral_Object* evaluate_binary_operator(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_OperatorID operator,
	const Ral_ExprNode* const left,
	const Ral_ExprNode* const right
)
{
	Ral_Object* leftobj = evaluate_node(state, local_variables, left);
	Ral_Object* rightobj = evaluate_node(state, local_variables, right);

	Ral_Object* result = NULL;

	switch (operator)
	{
	case Ral_OPERATOR_ADDITION:			result = Ral_Object_Add		(leftobj, rightobj); break;
	case Ral_OPERATOR_SUBTRACTION:		result = Ral_Object_Subtract(leftobj, rightobj); break;
	case Ral_OPERATOR_MULTIPLICATION:	result = Ral_Object_Multiply(leftobj, rightobj); break;
	case Ral_OPERATOR_DIVISION:			result = Ral_Object_Divide	(leftobj, rightobj); break;
	
	case Ral_OPERATOR_EQUALITY:			result = Ral_Object_Equality(leftobj, rightobj); break;

	default:
		printf("Binary operator not implemented!\n");
		break;
	}

	Ral_DestroyObject(leftobj);
	Ral_DestroyObject(rightobj);
	if (!result)
	{
		printf("Could not evaluate binary operator!\n");
		return NULL;
	}

	return result;
}



static Ral_Object* evaluate_assignment_operator(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_OperatorID operator,
	const Ral_ExprNode* const left,
	const Ral_ExprNode* const right
)
{
	if (left->type != Ral_EXPRNODETYPE_VARIABLE)
	{
		RalCLI_ERROR("NONO\n");
		return NULL;
	}

	Ral_Object* result = evaluate_node(state, local_variables, right);
	// Find the variable
	//Ral_Object* var = Ral_GetVariable
	// Assign the number to IT
	return result;
}



static Ral_Object* evaluate_unary_operator(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_OperatorID operator,
	const Ral_ExprNode* const left,
	const Ral_ExprNode* const right
)
{
	switch (operator)
	{
	case Ral_OPERATOR_NEGATIVE:
		return Ral_Object_Negative(right);
		break;
	default:
		return NULL;
		break;
	}
}



static Ral_Object* evaluate_node(
	Ral_State* const state,
	Ral_List* const local_variables,
	Ral_ExprNode* const node
)
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
		Ral_OperatorID op = token->operatorid;
		if (op == Ral_NOT_OPERATOR) return NULL;

		Ral_ExprNode* left_node = node->left;
		Ral_ExprNode* right_node = node->right;

		if (Ral_IS_UNARY_OPERATOR(op))
		{
			// Unary operator
			return evaluate_unary_operator(state, local_variables, op, left_node, right_node);
		} else if (!Ral_IS_ASSIGNMENT_OPERATOR(op))
		{
			// Binary operator
			return evaluate_binary_operator(state, local_variables, op, left_node, right_node);
		} else if (Ral_IS_ASSIGNMENT_OPERATOR(op))
		{
			// Assignment operator
			return evaluate_assignment_operator(state, local_variables, op, left_node, right_node);
		}
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

		Ral_Function* function = 0;

		// Make a list of objects for the results for the parameters
		Ral_ExprNode* parameter_node = node->functioncall_parameters.begin;
		while (parameter_node)
		{
			depth += 2;
			evaluate_node(state, local_variables, parameter_node);
			depth -= 2;
			parameter_node = parameter_node->next;
		}

		return Ral_CreateIntObjectFromInt(0);
		break;

	default:
		break;
	}
}



Ral_Object* Ral_EvaluateExpression(Ral_State* const state, Ral_List* const local_variables, const Ral_Expression* const expression)
{
	Ral_Object* result = evaluate_node(state, local_variables, expression->top);

	return result;
}

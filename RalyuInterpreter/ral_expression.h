#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"
#include "ral_object.h"



typedef struct Ral_ExprFunctionCall
{
	Ral_LISTLINKS(Ral_ExprFunctionCall);

	// List of expressions
	Ral_List paramater_expressions;
} Ral_ExprFunctionCall;

typedef struct Ral_ExprNode
{
	struct Ral_ExprNode* parent;
	struct Ral_ExprNode* left;
	struct Ral_ExprNode* right;

	const Ral_Token* corresp_token;

	Ral_ExprFunctionCall* functioncall;

	Ral_Object* expr_value;
} Ral_ExprNode;



typedef struct Ral_Expression
{
	Ral_LISTLINKS(Ral_Expression);

	// List of every function call in the expression
	Ral_List functioncalls;

	Ral_ExprNode* topnode;
} Ral_Expression;



Ral_Expression* Ral_BuildExpressionTree(
	const Ral_Statement* const statement,
	const int being,
	const int end
);



typedef struct
{
	Ral_ExprNode* node;
} Ral_ExpressionEvalState;

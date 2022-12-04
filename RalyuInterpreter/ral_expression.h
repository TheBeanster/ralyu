#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"
#include "ral_object.h"



typedef struct Ral_ExprNode
{
	struct Ral_ExprNode* parent;
	struct Ral_ExprNode* left;
	struct Ral_ExprNode* right;

	const Ral_Token* corresp_token;

	Ral_Object* expr_value;
} Ral_ExprNode;



typedef struct Ral_Expression
{
	Ral_List subexpressions;
} Ral_Expression;



Ral_Expression* Ral_CreateExpression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
);

Ral_Object* Ral_EvaluateExpression(
	Ral_Expression* const expression
);

void Ral_DestroyExpression(
	Ral_Expression* const expression
);

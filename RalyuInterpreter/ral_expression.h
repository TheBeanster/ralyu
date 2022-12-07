#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"
#include "ral_object.h"
#include "ral_execute.h"



typedef enum
{
	Ral_EXPRNODETYPE_NULL,
	Ral_EXPRNODETYPE_OPERATOR,
	Ral_EXPRNODETYPE_LITERAL,
	Ral_EXPRNODETYPE_VARIABLE,
	Ral_EXPRNODETYPE_FUNCTION
} Ral_ExprNodeType;



typedef struct Ral_ExprNode
{
	Ral_LISTLINKS(Ral_ExprNode);

	struct Ral_ExprNode* parent;
	struct Ral_ExprNode* left;
	struct Ral_ExprNode* right;

	Ral_Token* corresp_token;

	Ral_ExprNodeType type;

	// List of Ral_ExprNodes. The top node of every tree of the parameters.
	Ral_List functioncall_parameters;
} Ral_ExprNode;



void Ral_DestroyExprNode(Ral_ExprNode* const node);



typedef struct Ral_Expression
{
	Ral_ExprNode* top;
} Ral_Expression;



Ral_Expression* Ral_CreateExpression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
);

void Ral_DestroyExpression(
	Ral_Expression* const expression
);



Ral_Object* Ral_EvaluateExpression(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_Expression* const expression
);

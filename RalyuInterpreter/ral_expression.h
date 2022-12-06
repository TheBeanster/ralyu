#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"
#include "ral_object.h"
#include "ral_stack.h"



typedef enum
{
	Ral_EXPRNODETYPE_NULL,
	Ral_EXPRNODETYPE_OPERATOR,
	Ral_EXPRNODETYPE_NUMBERLITERAL,
	Ral_EXPRNODETYPE_VARIABLE,
	Ral_EXPRNODETYPE_FUNCTION
} Ral_ExprNodeType;



typedef struct Ral_ExprNode
{
	struct Ral_ExprNode* parent;
	struct Ral_ExprNode* left;
	struct Ral_ExprNode* right;

	const Ral_Token* corresp_token;

	Ral_ExprNodeType type;

	Ral_Object* expr_value;
} Ral_ExprNode;



typedef struct Ral_ExprFunctionCall
{
	Ral_LISTLINKS(Ral_ExprFunctionCall);

	Ral_ExprNode* node;
} Ral_ExprFunctionCall;



typedef struct Ral_SubExpression
{
	Ral_List functioncalls;
};



typedef enum
{
	Ral_EXPRFLAG_HASFUNCTIONCALLS = (1U << 0)
} Ral_ExprFlags;

typedef struct Ral_Expression
{
	Ral_ExprFlags flags;
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



Ral_StackNode* Ral_CreateExpressionStackNode(
	const Ral_Expression* const expression
);

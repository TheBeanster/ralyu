#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"



typedef enum
{
	Ral_STACKNODETYPE_NULL,
	Ral_STACKNODETYPE_EXPRESSIONEVAL,
} Ral_StackNodeType;

typedef struct Ral_StackNode
{
	Ral_LISTLINKS(Ral_StackNode);

	Ral_StackNodeType type;


} Ral_StackNode;

typedef struct
{
	Ral_List nodes;
} Ral_Stack;
#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_stack.h"
#include "ral_lexer.h"



// There should always be an expression stack node before function call nodes
typedef struct
{
	Ral_StackNode base;

	Ral_Statement* caller;
} Ral_StackNode_FunctionCall;





#pragma once

#include "ralu_stdinclude.h"

#include "ral_state.h"
#include "ral_object.h"
#include "ral_lexer.h"



Ral_Object* Ral_EvaluateExpression(
	Ral_State* const state,
	Ral_List* const local_variables,
	const Ral_Token* const tokens,
	const int begin,
	const int end
);



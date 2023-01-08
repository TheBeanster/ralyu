#pragma once

#include "ralu_stdinclude.h"

#include "ral_state.h"
#include "ral_object.h"



Ral_Object* Ral_GetTokenValue(
	const Ral_State* const state,
	const Ral_Token* const token
);



Ral_Object* Ral_EvaluateExpression(
	Ral_State* const state,
	const Ral_Token* const tokens,
	const int begin,
	const int end
);

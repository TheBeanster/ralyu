#pragma once

#include "ralu_stdinclude.h"

#include "ral_state.h"
#include "ral_sourceunit.h"
#include "ral_object.h"



Ral_Object* Ral_ExecuteSourceUnit(
	Ral_State* const state,
	const Ral_SourceUnit* const source
);



struct Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const struct Ral_Statement* const statement,
	Ral_List* const scope_variables,
	const Ral_Bool global,
	Ral_Object** const return_object
);

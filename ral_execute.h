#pragma once

#include "ralu_stdinclude.h"

#include "ral_state.h"
#include "ral_sourceunit.h"
#include "ral_object.h"



void Ral_ExecuteGlobalSourceUnit(
	Ral_State* const state,
	const Ral_SourceUnit* const source
);



Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const scope_variables,
	Ral_Object** const return_object
);

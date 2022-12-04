#pragma once

#include "ralu_stdinclude.h"

#include "ral_lexer.h"
#include "ral_sourceunit.h"
#include "ral_state.h"



/// @brief This function will contain all code executing the main program, including loading extra files, lexing and parsing source code
/// and the main execution loop.
/// @param sourceunit The main source unit where execution will start
void Ral_ExecuteSource(const Ral_SourceUnit* const sourceunit);



Ral_Bool Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement
);

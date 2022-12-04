#pragma once

#include "ralu_stdinclude.h"

#include "ral_sourceunit.h"
#include "ral_lexer.h"



typedef struct
{
	Ral_SourceUnit* mainsource;

	// Execution position
	Ral_SourceUnit*	current_sourceunit; // Which sourceunit execution is currently in
	int				current_statementid; // The statement index of current_statement
	Ral_Statement*	current_statement; // Pointer to the current statement being executed
	
} Ral_State;



Ral_State* Ral_CreateState(
	const Ral_SourceUnit* const mainsource
);

Ral_Bool Ral_SetStateExecutionPosition(
	Ral_State* const state,
	const Ral_SourceUnit* const sourceunit,
	const int statementid
);

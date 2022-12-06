#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"
#include "ral_sourceunit.h"
#include "ral_object.h"
#include "ral_stack.h"



typedef struct Ral_Scope
{
	Ral_LISTLINKS(Ral_Scope);

	Ral_List variables;
} Ral_Scope;



typedef struct
{
	Ral_SourceUnit* mainsource;

	// Execution position
	Ral_SourceUnit*	current_sourceunit; // Which sourceunit execution is currently in
	int				current_statementid; // The statement index of current_statement
	Ral_Statement*	current_statement; // Pointer to the current statement being executed

	Ral_Stack stack;

	Ral_List global_types;

	Ral_Scope scopestack;
} Ral_State;

Ral_State* Ral_CreateState(
	const Ral_SourceUnit* const mainsource
);

Ral_Bool Ral_SetStateExecutionPosition(
	Ral_State* const state,
	const Ral_SourceUnit* const sourceunit,
	const int statementid
);



/// @brief This function will contain all code executing the main program, including loading extra files, lexing and parsing source code
/// and the main execution loop.
/// @param sourceunit The main source unit where execution will start
void Ral_ExecuteSource(const Ral_SourceUnit* const sourceunit);



Ral_Bool Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement
);



Ral_Bool Ral_GotoNextStatement(
	Ral_State* const state
);

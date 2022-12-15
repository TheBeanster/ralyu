#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"
#include "ral_sourceunit.h"
#include "ral_object.h"



typedef struct
{
	Ral_SourceUnit* mainsource;

	Ral_List types;
	Ral_List arraytypes;
	Ral_List functions;

	Ral_List global_variables;
} Ral_State;

Ral_State* Ral_CreateState(
	const Ral_SourceUnit* const mainsource
);



/// @brief This function will contain all code executing the main program, including loading extra files, lexing and parsing source code
/// and the main execution loop.
/// @param sourceunit The main source unit where execution will start
void Ral_ExecuteSource(const Ral_SourceUnit* const sourceunit);



/// @brief 
/// @param state 
/// @param statement The statement to execute.
/// @param local_variables 
/// @param return_object A pointer to the object that this execution should return.
/// @return A pointer to the next statement to execute, or NULL if the execution should stop.
Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const local_variables,
	Ral_Object** return_object
);



Ral_Bool Ral_GotoNextStatement(
	Ral_State* const state
);


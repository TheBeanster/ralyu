#pragma once

#include "ralu_stdinclude.h"

#include "ral_lexer.h"
#include "ral_sourceunit.h"



typedef struct
{
	int current_statementid;
	Ral_Statement* current_statement;
} Ral_State;

void Ral_ExecuteSource(const Ral_SourceUnit* const sourceunit);

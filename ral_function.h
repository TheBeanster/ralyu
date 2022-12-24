#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_sourceunit.h"



typedef struct Ral_Function
{
	Ral_LISTLINKS(Ral_Function);
	
	Ral_SourceUnit* source;
} Ral_Function;
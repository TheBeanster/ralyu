#include "ral_sourceunit.h"

#include "ralu_memory.h"
#include <stdio.h>

#include "ral_lexer.h"



Ral_SourceUnit* Ral_LoadSourceString(
	Ral_State* const state,
	const char* const string,
	const int length
)
{
	Ral_SourceUnit* sourceunit = Ral_ALLOC_TYPE(Ral_SourceUnit);
	if (!Ral_ParseSourceUnit(sourceunit, string, length))
	{
		printf("Could not parse source code!\n");
		Ral_FREE(sourceunit);
		return NULL;
	}

	

	return sourceunit;
}

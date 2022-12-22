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



void Ral_DestroySourceUnit(
	Ral_SourceUnit* const sourceunit
)
{
	if (!sourceunit) return;
	if (sourceunit->numstatements > 0 && sourceunit->statements != NULL)
	{
		for (int i = 0; i < sourceunit->numstatements; i++)
		{
			Ral_Statement* s = &sourceunit->statements[i];
			for (int j = 0; j < s->numtokens; j++)
			{
				if (s->tokens[j].string)
					Ral_FREE(s->tokens[j].string);
			}
			Ral_FREE(s->tokens);
		}
	}
	Ral_FREE(sourceunit->statements);
	Ral_FREE(sourceunit);
}



void Ral_PrintSourceUnit(
	const Ral_SourceUnit* const sourceunit
)
{
	for (int i = 0; i < sourceunit->numstatements; i++)
	{
		Ral_PrintStatementTokens(&sourceunit->statements[i]);
		putchar('\n');
	}
}
#include "ral_sourceunit.h"

#include "ralu_memory.h"
#include <stdio.h>

#include "ral_lexer.h"
#include "ral_state.h"



Ral_SourceUnit* Ral_CreateSourceUnit(const Ral_List* const statements)
{
	Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);

	source->numstatements = statements->itemcount;
	source->statements = Ral_CALLOC(source->numstatements, sizeof(Ral_Statement));
	source->prev = NULL;
	source->next = NULL;

	Ral_Statement* iterator = statements->begin;
	for (int i = 0; i < source->numstatements; i++)
	{
		Ral_Statement* s = &source->statements[i];
		s->index = i;
		s->numtokens = iterator->numtokens;
		s->tokens = iterator->tokens;
		s->type = iterator->type;
		s->prev = NULL;
		s->next = NULL;
		s->parentsource = source;

		iterator = iterator->next;
	}

	return source;
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

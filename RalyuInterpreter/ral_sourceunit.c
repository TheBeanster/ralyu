#include "ral_sourceunit.h"

#include "ralu_memory.h"

#include "ral_logging.h"



Ral_SourceUnit* Ral_CreateSourceUnit(
	const Ral_TextFile* const textfile
)
{
	if (!textfile) return NULL;

	Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);

	source->file = textfile;
	source->buffer = textfile->buffer;
	source->length = textfile->length;

	return source;
}



void Ral_DestroySourceUnit(
	Ral_SourceUnit* const sourceunit
)
{
	for (int i = 0; i < sourceunit->numstatements; i++)
	{
		// An error occurs here when destroying the statement tokens.
		// TODO FIX ERROR
		Ral_DestroyStatement(&sourceunit->statements[i]);
	}
	Ral_ClearList(&sourceunit->errormessages, &Ral_DestroyErrorMessage);
	Ral_FREE(sourceunit);
}
#include "ral_sourceunit.h"

#include "ral_memory.h"



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

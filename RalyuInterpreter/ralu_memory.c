#include "ralu_memory.h"



#if !RalCompile_USE_STD_MALLOC

#include <stdlib.h>
#include <string.h>



void* Ral_Malloc(const size_t size)
{
	void* block = malloc(size);
	if (!block) exit(1);

	memset(block, 0, size);

	return block;
}



void* Ral_Calloc(const int count, const size_t size)
{
	void* block = calloc(count, size);
	if (!block) exit(1);

	return block;
}



void* Ral_Free(const void* const block)
{
	free(block);
}



#endif
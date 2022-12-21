#include "ralu_memory.h"



#if !Ral_COMPILE_USE_STD_MALLOC

void* Ral_Malloc(const size_t size)
{
	void* block = malloc(size);
	if (!block) exit(1);
	return block;
}

void* Ral_Calloc(const int count, const size_t size)
{
	void* block = calloc(count, size);
	if (!block) exit(1);
	return block;
}

void* Ral_Realloc(void* block, const size_t size)
{
	void* newblock = realloc(block, size);
	if (!newblock) exit(1);
	return newblock;
}

void* Ral_Free(const void* const block)
{
	free(block);
}

#endif
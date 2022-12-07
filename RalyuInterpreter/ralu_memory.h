#pragma once

#include "ralu_stdinclude.h"

#define RalCompile_USE_STD_MALLOC 1



#if RalCompile_USE_STD_MALLOC

#include <stdlib.h>
#include <string.h>

#define Ral_MALLOC(size)				malloc(size)
#define Ral_CALLOC(count, size)			calloc(count, size)

#define	Ral_ALLOC_TYPE(type)			(type*)calloc(1, sizeof(type))

#define Ral_FREE(block)					free(block)

#else

#include <stdlib.h>

void* Ral_Malloc(const size_t size);
void* Ral_Calloc(const int count, const size_t size);
void* Ral_Free(const void* const block);

#define Ral_MALLOC(size)				Ral_Malloc(size)
#define Ral_CALLOC(count, size)			Ral_Calloc(count, size)

#define	Ral_ALLOC_TYPE(type)			Ral_Calloc(1, sizeof(type))

#define Ral_FREE(block)					Ral_Free(block)

#endif

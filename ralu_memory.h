#pragma once

#include "ralu_stdinclude.h"



// Ralyu memory management utilities



// Compile memory management functions with raw standard library function calls
// or call ralyu functions which have error checking.
// Compiling the normal standard library functions could be useful in some IDEs
// that keep track of allocations by checking calls to the standard functions.
#define Ral_COMPILE_USE_STD_MALLOC 1

#if Ral_COMPILE_USE_STD_MALLOC

#define Ral_MALLOC(size)			malloc(size)
#define Ral_CALLOC(count, size)		calloc(count, size)
#define Ral_REALLOC(block, size)	realloc(block, size)
#define	Ral_ALLOC_TYPE(type)		(type*)calloc(1, sizeof(type))
#define Ral_FREE(block)				free(block)

#else

void* Ral_Malloc(const size_t size);
void* Ral_Calloc(const int count, const size_t size);
void* Ral_Realloc(void* block, const size_t size);
void* Ral_Free(const void* const block);

#define Ral_MALLOC(size)			Ral_Malloc(size)
#define Ral_CALLOC(count, size)		Ral_Calloc(count, size)
#define	Ral_ALLOC_TYPE(type)		Ral_Calloc(1, sizeof(type))
#define Ral_FREE(block)				Ral_Free(block)

#endif
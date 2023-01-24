#pragma once

#include <stdlib.h>

#if _DEBUG
#define Ral_DEBUG
#else
#define Ral_RELEASE
#endif

typedef enum Ral_Bool
{
	Ral_FALSE = 0,
	Ral_TRUE = 1
} Ral_Bool;

#define Ral_STRINGIFY(str) #str



#ifdef Ral_USE_DEBUG_PRINT
#define DEBUG_PRINT(format, ...) printf(format, __VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) void
#endif

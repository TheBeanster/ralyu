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
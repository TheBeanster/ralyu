#pragma once

#ifdef _DEBUG
#define Ral_DEBUG
#endif
#ifdef _RELEASE
#define Ral_RELEASE
#endif

#ifdef _WIN64
#define Ral_X64
#else
#ifdef _WIN32
#define Ral_X86
#endif
#endif



#define NULL 0

typedef enum
{
	Ral_FALSE = 0,
	Ral_TRUE = 1
} Ral_Bool;

#define Ral_ARRAYLEN(arr) (sizeof(arr) / sizeof(*arr))

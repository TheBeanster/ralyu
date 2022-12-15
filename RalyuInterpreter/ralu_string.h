#pragma once

#include "ralu_stdinclude.h"

Ral_Bool Ral_IsAlpha(char c);

Ral_Bool Ral_IsNumber(char c);

Ral_Bool Ral_IsCharIn(
	const char c,
	const char* const string
);


char* Ral_ToLowercase(
	char* const string
);

int Ral_GetIndexOfString(
	const char* const inputstring,
	const char** const tokens,
	const int numtokens
);

char* Ral_CreateCutString(
	const char* const	srcstring,
	const int			start,
	const int			length
);

char* Ral_CreateMergeStrings(
	const char* const	str1,
	const char* const	str2
);

/// @brief Prints a string while removing spacing, ' ', '\t' and '\n'.
/// @param string Pointer to a string
void Ral_PrintCondensedString(
	const char* const	string
);

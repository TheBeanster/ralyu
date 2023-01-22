#pragma once

#include "ralu_stdinclude.h"
#include <string.h>
#include <ctype.h>



char* Ral_CreateCutString(
	const char* const	srcstring,
	const int			start,
	const int			length
);



// Vector string
typedef struct Ral_VString
{
	char* chars;
	int length;
	int capacity;
} Ral_VString;

void Ral_VStringInit(Ral_VString* const vstring, const char* const str);

void Ral_VStringAppend(Ral_VString* const vstring, const char* const str);

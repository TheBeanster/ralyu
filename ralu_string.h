#pragma once

#include "ralu_stdinclude.h"
#include <string.h>
#include <ctype.h>



char* Ral_CreateCutString(
	const char* const	srcstring,
	const int			start,
	const int			length
);



typedef struct Ral_RString
{
	char* chars;
	int length;
} Ral_RString;

void Ral_RStringInitEmpty(Ral_RString* const rstring);

void Ral_RStringInit(Ral_RString* const rstring, const char* const str);

void Ral_RStringAppend(Ral_RString* const rstring, const char* const str);

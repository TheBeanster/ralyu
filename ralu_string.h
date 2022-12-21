#pragma once

#include "ralu_stdinclude.h"
#include <string.h>
#include <ctype.h>



char* Ral_CreateCutString(
	const char* const	srcstring,
	const int			start,
	const int			length
);



#define Ral_STR_COMMA_LEN(str) str, strlen(str)

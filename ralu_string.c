#include "ralu_string.h"

#include "ralu_memory.h"



char* Ral_CreateCutString(
	const char* const	srcstring,
	const int			start,
	const int			length
)
{
	char* str = Ral_MALLOC(length + 1); // Plus 1 to include null terminator

	for (int i = 0; i < length; i++)
	{
		str[i] = srcstring[i + start];
	}

	str[length] = '\0';
	return str;
}
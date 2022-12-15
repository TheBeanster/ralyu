#include "ralu_string.h"

#include <stdio.h>
#include "ralu_memory.h"



Ral_Bool Ral_IsAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}



Ral_Bool Ral_IsNumber(char c)
{
	if (c >= '0' && c <= '9')
		return Ral_TRUE;
	else
		return Ral_FALSE;
}



Ral_Bool Ral_IsCharIn(
	const char c,
	const char* const string
)
{
	int i = 0;
	while (1)
	{
		char test = string[i];
		if (test == '\0') return Ral_FALSE;
		if (c == test) return Ral_TRUE;
		i++;
	}
}



char* Ral_ToLowercase(
	char* const string
)
{
	int i = 0;
	while (1)
	{
		char c = string[i];
		if (c == '\0') return string;
		else if (Ral_IsAlpha(c))
		{
			string[i] |= 0b00100000;
		}
		i++;
	}
}



int Ral_GetIndexOfString(
	const char* const inputstring,
	const char** const tokens,
	const int numtokens
)
{
	for (int i = 0; i < numtokens; i++)
	{
		if (strcmp(inputstring, tokens[i]) == 0)
			return i;
	}
	return -1;
}



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



char* Ral_CreateMergeStrings(const char* const str1, const char* const str2)
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	int length = len1 + len2;
	char* dststr = Ral_MALLOC(length + 1); // Plus 1 to include null terminator

	for (int i = 0; i < len1; i++)
		dststr[i] = str1[i];
	for (int i = 0; i < len2; i++)
		dststr[i + len1] = str2[i];

	dststr[length] = '\0';
	return dststr;
}



void Ral_PrintCondensedString(const char* const string)
{
	char* c = string;
	Ral_Bool prev_was_spacer = Ral_TRUE;
	while (1)
	{
		switch (*c)
		{
		case '\0':
			return;

		case ' ':
		case '\n':
		case '\t':
			if (!prev_was_spacer)
			{
				putchar(' ');
				prev_was_spacer = Ral_TRUE;
			}
			break;

		default:
			putchar(*c);
			prev_was_spacer = Ral_FALSE;
			break;
		}
		c++;
	}
}

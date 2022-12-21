/*
This file is here to test if Ralyu has compiled and is running correctly.
To embedd Ralyu in a program, simply delete this file and setup Ralyu in that program.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "ralu_stdinclude.h"
#include "ralu_list.h"
#include "ralu_memory.h"
#include "ralu_string.h"
#include <stdio.h>

#include "ral_lexer.h"



int main(int argc, char** argv)
{
	printf("Test implementation of Ralyu\n");

	Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);
	Ral_ParseSourceUnit(source, Ral_STR_COMMA_LEN("0.0.0"));

	/*while (1)
	{
		char input[101];
		printf("> ");
		gets_s(input, 100);
		Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);
		Ral_ParseSourceUnit(source, input, strlen(input));
	}*/
}

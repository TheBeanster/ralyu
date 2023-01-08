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

#include "ral_state.h"
#include "ral_sourceunit.h"
#include "ral_execute.h"



int main(int argc, char** argv)
{
	printf("Test implementation of Ralyu\n");

	//Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);
	////"int a = 0; int b = a + 1; print(a); print(b)"
	//Ral_ParseSourceUnit(source, Ral_STR_COMMA_LEN("int a = 0"));
	//Ral_PrintSourceUnit(source);
	//Ral_DestroySourceUnit(source);
	
	/*while (1)
	{
		char input[101];
		printf("> ");
		gets_s(input, 100);
		Ral_SourceUnit* source = Ral_ALLOC_TYPE(Ral_SourceUnit);
		Ral_ParseSourceUnit(source, input, strlen(input));
		Ral_PrintSourceUnit(source);
		Ral_DestroySourceUnit(source);
	}*/

	Ral_State* state = Ral_CreateState();
	Ral_SourceUnit* source = Ral_LoadSourceString(state, "int a = 10 + 10\na = 1\nend");
	if (!source)
	{
		Ral_DestroyState(state);
		return 0;
	}

	Ral_PrintSourceUnit(source);

	Ral_ExecuteGlobalSourceUnit(state, source);

	Ral_DestroyState(state);

}

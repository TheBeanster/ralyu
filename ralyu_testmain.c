/*
This file is here to test if Ralyu has compiled and is running correctly.
To embedd Ralyu in a program, simply delete this file and setup Ralyu in that program.
*/

#include <stdio.h>

#include "ralu_file.h"
#include "ralu_memory.h"
#include "ral_state.h"
#include "ral_sourceunit.h"
#include "ral_execute.h"
#include "ral_function.h"
#include "ral_variable.h"
#include "ral_library.h"

#include "ral_stdlibrary.h"



Ral_Object* TestFunction(Ral_State* state, Ral_List* arguments)
{
	if (arguments->itemcount != 1)
	{
		state->errormsg = "Invalid arguments for print";
		return NULL;
	}
	Ral_Object* str = arguments->begin;

	/*if (str->type != Ral_TYPE_STRING)
	{
		state->errormsg = "Print expects a string";
		return NULL;
	}*/

	printf("TestFunction > %f\n", str->val.number);

	return NULL;
}



Ral_Library(testlib,
	Ral_LibFunction(,TestFunction)
	Ral_LibNumber("blip", 69)
);



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
	char* sourcecode = Ral_LoadFileString("test.ral");
	Ral_SourceUnit* source = Ral_LoadSourceString(state, sourcecode);
	Ral_FREE(sourcecode);

	if (!source)
	{
		Ral_DestroyState(state);
		return 0;
	}

	Ral_PrintSourceUnit(source);

	Ral_LoadLib_STDIO(state);
	Ral_LoadLib_STDMath(state);

	Ral_LoadLib(state, testlib);

	Ral_ExecuteSourceUnit(state, source);

	Ral_DestroyState(state);

}

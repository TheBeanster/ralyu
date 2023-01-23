#include "ral_stdlibrary.h"

#include <stdio.h>
#include "ralu_list.h"

#include "ral_object.h"
#include "ral_library.h"



static Ral_Object* stdio_print(Ral_State* const state, Ral_List* const arguments)
{
	if (arguments->itemcount <= 0) return NULL;
	
	Ral_PrintObjectValue(arguments->begin);
	putchar('\n');

	return NULL;
}



Ral_Library(stdio,
	Ral_LibFunction(stdio_, print)
);



Ral_LoadLib_STDIO(Ral_State* const state)
{
	Ral_LoadLib(state, stdio);
}
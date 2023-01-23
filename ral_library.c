#include "ral_library.h"

#include <stdio.h>

#include "ral_variable.h"



void Ral_LoadLibrary(Ral_State* const state, Ral_LibraryIdentifier lib[], const char* const libname)
{
	printf("Loading library \"%s\"\n", libname);

	Ral_LibraryIdentifier* iterator = lib;
	while (iterator)
	{
		if (iterator->type == Ral_LIBITYPE_END) return;

		if ((iterator + 1)->type != Ral_LIBITYPE_END)
			printf(" \xC3 ");
		else
			printf(" \xC0 ");
		
		switch (iterator->type)
		{
		case Ral_LIBITYPE_BOOL:
			printf("\"%s\"\x1B[30G= %s\n", iterator->name, iterator->boolean ? "true" : "false");
			Ral_SetGlobalBool(state, iterator->name, iterator->boolean);
			break;

		case Ral_LIBITYPE_NUMBER:
			printf("\"%s\"\x1B[30G= %f\n", iterator->name, iterator->number);
			Ral_SetGlobalNumber(state, iterator->name, iterator->number);
			break;

		case Ral_LIBITYPE_FUNCTION:
			printf("\"%s\"\x1B[30G= function\n", iterator->name);
			Ral_DeclareCFunction(state, iterator->function, iterator->name);
			break;

		case Ral_LIBITYPE_END: return;
		}

		iterator+=1;
	}
}

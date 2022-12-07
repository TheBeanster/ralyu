#include "ral_object.h"

#include <string.h>

#include "ral_execute.h"



const Ral_Type ral_base_types[] =
{
	{NULL, NULL, "int", Ral_TYPETAG_BASE},
	{NULL, NULL, "float", Ral_TYPETAG_BASE},
};



Ral_Type* Ral_GetType(
	const struct Ral_State* const state,
	const char* const str
)
{
	for (int i = 0; i < Ral_NUMBASETYPES; i++)
	{
		if(strcmp(str, ral_base_types[i].name) == 0)
		{
			return &ral_base_types[i];
		}
	}

	Ral_Type* iterator = ((Ral_State*)state)->global_types.begin;
	while (iterator)
	{
		if (strcmp(str, iterator->name) == 0)
			return iterator;
		
		iterator = iterator->next;
	}

	return NULL;
}
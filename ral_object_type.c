#include "ral_object.h"

#include "ralu_string.h"



const Ral_Type ral_base_types[] =
{
	{NULL,	NULL,	Ral_TYPETAG_INTEGER,	"int"},
	{NULL,	NULL,	Ral_TYPETAG_FLOAT,		"float"},
	{NULL,	NULL,	Ral_TYPETAG_STRING,		"string"}
};



Ral_Type* Ral_GetType(
	const Ral_State* const state,
	const char* const string
)
{
	for (int i = 0; i < Ral_NUM_BASE_TYPES; i++)
	{
		if (strcmp(string, ral_base_types[i].name) == 0)
			return &ral_base_types[i];
	}

	Ral_Type* iterator = state->global_types.begin;
	while (iterator)
	{
		if (strcmp(string, iterator->name) == 0)
			return iterator;
	}

	return NULL;
}
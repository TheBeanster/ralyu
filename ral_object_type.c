#include "ral_object.h"

#include "ralu_string.h"



const Ral_Type ral_base_types[] =
{
	{NULL,	NULL,	Ral_TYPETAG_INTEGER,	"int",			NULL},
	{NULL,	NULL,	Ral_TYPETAG_FLOAT,		"float",		NULL},
	{NULL,	NULL,	Ral_TYPETAG_STRING,		"string",		NULL}
};



Ral_Type* Ral_GetTypeS(
	const Ral_State* const state,
	const char* const string
)
{
	if (!string) return NULL;

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



Ral_Type* Ral_GetTypeT(const Ral_State* const state, const Ral_Token* const tokens, const int begin, const int end)
{
	if (!(tokens[begin].type == Ral_TOKENTYPE_IDENTIFIER || tokens[begin].type == Ral_TOKENTYPE_KEYWORD)) return NULL;
	if (begin + 1 >= end) return Ral_GetTypeS(state, tokens[begin].string);

	// Read array []
	int num_lbrackets = 0;
	for (int i = begin; i < end; i++)
	{
		if (tokens[i].separatorid == Ral_SEPARATOR_LBRACKET) num_lbrackets++;
	}

	if (num_lbrackets == 1)
	{

	}

	return NULL;
}

#include "ral_object.h"

#include "ralu_memory.h"
#include "ralu_string.h"
#include <string.h>

#include "ral_execute.h"



static Ral_Bool typename_is_array(const char* str)
{
	char c;
	do
	{
		c = *str;
		if (c == '\0') return Ral_FALSE;
		str++;
	}
	while (c != '[');
	return Ral_TRUE;
}



const Ral_Type ral_base_types[] =
{
	{NULL, NULL, "int", Ral_TYPETAG_BASE},
	{NULL, NULL, "float", Ral_TYPETAG_BASE},
	{NULL, NULL, "string", Ral_TYPETAG_BASE},
};



Ral_Type* Ral_DeclareArrayType(const Ral_State* const state, const Ral_Type* const elementtype)
{
	Ral_ArrayType* type = Ral_ALLOC_TYPE(Ral_ArrayType);
	type->base.name = Ral_CreateMergeStrings(elementtype->name, "[]");
	type->base.tag = Ral_TYPETAG_ARRAY;
	type->elementtype = elementtype;
	Ral_PushFrontList(&state->arraytypes, type);
	return type;
}



Ral_Type* Ral_DeclareStructType(const Ral_State* const state, const char* const str, const Ral_List* const members)
{
	Ral_StructType* type = Ral_ALLOC_TYPE(Ral_StructType);
	type->base.name = _strdup(str);
	type->base.tag = Ral_TYPETAG_STRUCT;
	type->members = members;
	Ral_PushFrontList(&state->arraytypes, type);
	return type;
}



Ral_Type* Ral_GetType(
	const struct Ral_State* const state,
	const char* const str
)
{
	// Check if typename is an array type
	if (typename_is_array(str))
	{
		// Only look at the array types in state
		Ral_Type* iterator = ((Ral_State*)state)->arraytypes.begin;
		while (iterator)
		{
			if (strcmp(str, iterator->name) == 0)
				return iterator;

			iterator = iterator->next;
		}
	}

	// Check if typename is a base type
	for (int i = 0; i < Ral_NUMBASETYPES; i++)
	{
		if(strcmp(str, ral_base_types[i].name) == 0)
		{
			return &ral_base_types[i];
		}
	}

	// Check if typename is any of the other types in the state
	Ral_Type* iterator = ((Ral_State*)state)->types.begin;
	while (iterator)
	{
		if (strcmp(str, iterator->name) == 0)
			return iterator;
		
		iterator = iterator->next;
	}

	return NULL;
}

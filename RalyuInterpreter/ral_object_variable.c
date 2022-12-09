#include "ral_object.h"

#include <string.h>

#include "ral_execute.h"
#include "ral_cli.h"



Ral_Object* Ral_DeclareVariable(
	Ral_List* const			list,
	const char* const		name,
	const Ral_Type* const	type
)
{
	RalCLI_DEBUGLOG("Declaring variable \"%s\"", name);
	Ral_Object* obj = Ral_CreateObject(type);
	obj->name = name;
	Ral_PushFrontList(list, obj);
}



Ral_Object* Ral_GetVariable(
	const struct Ral_State* const	state,
	const Ral_List* const			local_variables,
	const char* const				name
)
{
	Ral_Object* iterator = local_variables->begin;
	while (iterator)
	{
		if (strcmp(name, iterator->name) == 0)
			return iterator;
		iterator = iterator->next;
	}

	iterator = ((Ral_State*)state)->global_variables.begin;
	while (iterator)
	{
		if (strcmp(name, iterator->name) == 0)
			return iterator;
		iterator = iterator->next;
	}

	return NULL;
}

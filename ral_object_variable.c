#include "ral_object.h"

#include "ralu_memory.h"
#include "ralu_string.h"
#include <stdio.h>



Ral_Object* Ral_DeclareVariable(
	Ral_List* const scope,
	const Ral_Type* const type,
	const char* const name
)
{
	printf("Declaring new variable \"%s\" of type \"%s\"\n", name, type->name);
	Ral_Object* var = Ral_CreateObject(type);
	var->name = _strdup(name);
	Ral_PushBackList(scope, var);
	return var;
}
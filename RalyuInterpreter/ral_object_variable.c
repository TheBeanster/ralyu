#include "ral_object.h"

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

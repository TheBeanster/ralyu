#include "ral_object.h"



Ral_Object* Ral_DeclareVariable(
	Ral_List* const			list,
	const char* const		name,
	const Ral_Type* const	type
)
{
	Ral_Object* obj = Ral_CreateObject(type);
	obj->name = name;
	Ral_PushFrontList(list, obj);
}

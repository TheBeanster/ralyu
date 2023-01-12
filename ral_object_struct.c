#include "ral_object.h"

#include "ralu_memory.h"



Ral_Object* Ral_CreateStructObject()
{
	Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
	obj->type = Ral_TYPE_STRUCT;
	obj->val.struc.members.begin = NULL;
	obj->val.struc.members.end = NULL;
	obj->val.struc.members.itemcount = NULL;
	return obj;
}



Ral_Bool Ral_AddStructMember(Ral_Object* const obj, const char* const name, const Ral_Object* const val)
{
	if (!(obj && name)) return Ral_FALSE;

	Ral_StructMember* member = Ral_ALLOC_TYPE(Ral_StructMember);
	member->name = _strdup(name);
	member->obj = Ral_CopyObject(val);
	Ral_PushBackList(&obj->val.struc.members, member);
	return Ral_TRUE;
}



Ral_StructMember* Ral_GetStructMemberNode(const Ral_Object* const obj, const char* const name)
{
	if (!(obj && name)) return NULL;

	Ral_StructMember* iterator = obj->val.struc.members.begin;
	while (iterator)
	{
		if (strcmp(name, iterator->name) == 0) return iterator;
		iterator = iterator->next;
	}
	return NULL;
}



Ral_Object* Ral_GetStructMember(const Ral_Object* const obj, const char* const name)
{
	if (!(obj && name)) return NULL;

	Ral_StructMember* member = Ral_GetStructMemberNode(obj, name);
	if (!member) return NULL;
	return member->obj;
}



void Ral_RemoveStructMember(Ral_Object* const obj, Ral_StructMember* const member)
{
	if (!(obj && member)) return Ral_FALSE;

	Ral_UnlinkFromList(&obj->val.struc.members, member);
}

#include "ral_object.h"

#include "ralu_memory.h"

#include "ral_cli.h"



Ral_Object* Ral_CreateObject(const Ral_Type* const type)
{
	if (type == &ral_base_types[Ral_BASETYPE_INT])
	{
		Ral_Object_Int* object = Ral_ALLOC_TYPE(Ral_Object_Int);
		object->base.type = type;
		return object;
	}
	if (type == &ral_base_types[Ral_BASETYPE_FLOAT])
	{
		Ral_Object_Float* object = Ral_ALLOC_TYPE(Ral_Object_Float);
		object->base.type = type;
		return object;
	}

	RalCLI_ERROR("Tried to create object of invalid type");
	return NULL;
}



void Ral_DeleteObject(Ral_Object* const object)
{
	if (
		object->type == &ral_base_types[Ral_BASETYPE_INT] ||
		object->type == &ral_base_types[Ral_BASETYPE_FLOAT])
	{
		Ral_FREE(object);
	}
}



Ral_Object* Ral_CreateObjectFromLiteral(const Ral_Token* const token)
{
	if (token->type == Ral_TOKENTYPE_NUMBERLITERAL)
	{
		Ral_Object_Int* obj = Ral_CreateObject(&ral_base_types[Ral_BASETYPE_INT]);
		obj->value = token->numberliteral_value;
	}
	else if (token->type == Ral_TOKENTYPE_STRINGLITERAL)
	{

	}
	else
	{
		RalCLI_ERROR("Tried to evaluate literal value of token that was not literal!");
		return NULL;
	}
}

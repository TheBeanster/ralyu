#include "ral_object.h"

#include "ralu_memory.h"
#include "ralu_string.h"

#include "ral_cli.h"



Ral_Object* Ral_CreateObject(const Ral_Type* const type)
{
	if (type == Ral_TYPEINT)
	{
		Ral_Object_Int* object = Ral_ALLOC_TYPE(Ral_Object_Int);
		object->base.type = type;
		return object;
	}
	if (type == Ral_TYPEFLOAT)
	{
		Ral_Object_Float* object = Ral_ALLOC_TYPE(Ral_Object_Float);
		object->base.type = type;
		return object;
	}

	RalCLI_ERROR("Tried to create object of invalid type");
	return NULL;
}



void Ral_DestroyObject(Ral_Object* const object)
{
	if (object->type == Ral_TYPEINT ||
		object->type == Ral_TYPEFLOAT)
	{
		Ral_FREE(object);
	} else
	{
		Ral_FREE(object);
	}
}








Ral_Object_Int* Ral_CreateIntObjectFromInt(const int i)
{
	Ral_Object_Int* obj = Ral_CreateObject(Ral_TYPEINT);
	obj->value = i;
	return obj;
}

Ral_Object_Float* Ral_CreateFloatObjectFromFloat(const float f)
{
	Ral_Object_Float* obj = Ral_CreateObject(Ral_TYPEFLOAT);
	obj->value = f;
	return obj;
}

Ral_Object* Ral_CreateObjectFromLiteral(const Ral_Token* const token)
{
	if (token->type == Ral_TOKENTYPE_NUMBERLITERAL)
	{
		if (Ral_IsCharIn('.', token->string))
		{
			// Number literal has a .
			return Ral_CreateFloatObjectFromFloat(token->numberliteral_value);
		} else
		{
			// Number literal is an int
			return Ral_CreateIntObjectFromInt(token->numberliteral_value);
		}
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





Ral_Object* Ral_CopyObject(const Ral_Object* const obj)
{
	return NULL;
}

Ral_Object_Int* Ral_CopyIntObject(const Ral_Object_Int* const obj)
{
	return Ral_CreateIntObjectFromInt(obj->value);
}

Ral_Object_Float* Ral_CopyFloatObject(const Ral_Object_Float* const obj)
{
	return Ral_CreateFloatObjectFromFloat(obj->value);
}

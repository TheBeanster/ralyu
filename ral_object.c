#include "ral_object.h"

#include "ralu_memory.h"
#include "ralu_string.h"



Ral_Object* Ral_CreateIntObject(const int i)
{
	Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
	obj->value.integer = i;
	return obj;
}

Ral_Object* Ral_CreateFloatObject(const float i)
{
	Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
	obj->value.number = i;
	return obj;
}



Ral_Object* Ral_CreateObject(
	const Ral_Type* const type
)
{
	Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
	obj->type = type;
	switch (type->tag)
	{
	case Ral_TYPETAG_ARRAY:
		obj->value.arr.element_type = type->array_element_type;
		break;

	default:
		break;
	}

	return obj;
}
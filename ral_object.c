#include "ral_object.h"

#include "ralu_memory.h"



Ral_Object* Ral_CreateIntObject(const int i)
{
	Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
	obj->value.integer = i;
	return obj;
}

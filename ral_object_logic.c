#include "ral_object.h"



Ral_Object* Ral_ObjectAdd(const Ral_Object* const a, const Ral_Object* const b)
{
	// If either object is null
	if ((a && b) == 0) return NULL;

	if (a->type != b->type)
	{
		// Object have different types
		return NULL;
	} else
	{
		// Object have the same type
		switch (a->type)
		{

		case Ral_TYPE_NUMBER:
			return Ral_CreateNumberObject(a->val.number + b->val.number);

		default:
			break;
		}
	}
}

Ral_Object* Ral_ObjectSub(const Ral_Object* const a, const Ral_Object* const b)
{

}

Ral_Object* Ral_ObjectMul(const Ral_Object* const a, const Ral_Object* const b);
Ral_Object* Ral_ObjectDiv(const Ral_Object* const a, const Ral_Object* const b);

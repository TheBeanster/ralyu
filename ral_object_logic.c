#include "ral_object.h"

#include "ral_lexer.h"



Ral_Bool Ral_ObjectIsTrue(const Ral_Object* const obj)
{
	if (!obj) return Ral_FALSE;

	switch (obj->type)
	{
	case Ral_TYPE_NUMBER:
		return obj->val.number != 0;
		break;

	case Ral_TYPE_BOOL:
		return obj->val.boolean != Ral_FALSE;
		break;

	default:
		return Ral_FALSE;
		break;
	}
	return Ral_FALSE;
}



Ral_Object* Ral_ObjectOperator(Ral_OperatorID op, const Ral_Object* const a, const Ral_Object* b)
{
	return NULL;
}



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



Ral_Object* Ral_ObjectEqual(const Ral_Object* const a, const Ral_Object* const b)
{
	// Always give false if types are different. Temporary
	if (a->type != b->type) return Ral_CreateBoolObject(Ral_FALSE);

	switch (a->type)
	{
	case Ral_TYPE_BOOL:
		return Ral_CreateBoolObject(a->val.boolean == b->val.boolean);
		break;

	case Ral_TYPE_NUMBER:
		return Ral_CreateBoolObject(a->val.number == b->val.number);
		break;
	default:
		return 0xAAAAAAAAAA;
		break;
	}
}

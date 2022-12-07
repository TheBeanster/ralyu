#include "ral_object.h"



Ral_Object* Ral_Object_Add(
	Ral_Object* const a,
	Ral_Object* const b
)
{
	if (!a || !b) return NULL;

	if (a->type->tag != Ral_TYPETAG_BASE ||
		b->type->tag != Ral_TYPETAG_BASE)
	{
		// Must be base types
		return NULL;
	}

	if (Ral_ISBASETYPE(Ral_BASETYPE_INT, a))
	{
		if (Ral_ISBASETYPE(Ral_BASETYPE_INT, b))
		{
			Ral_Object_Int* int_a = (Ral_Object_Int*)a;
			Ral_Object_Int* int_b = (Ral_Object_Int*)a;
			int result = int_a->value + int_b->value;
			return Ral_CreateIntObjectFromInt(result);
		} else
			return NULL;
	} else
		return NULL;
}



Ral_Object* Ral_Object_Subtract(
	Ral_Object* const a,
	Ral_Object* const b
)
{

}



Ral_Object* Ral_Object_Multiply(
	Ral_Object* const a,
	Ral_Object* const b
)
{

}



Ral_Object* Ral_Object_Divide(
	Ral_Object* const a,
	Ral_Object* const b
)
{

}



Ral_Object* Ral_Object_Negative(
	Ral_Object* const a
)
{
	if (!a) return NULL;

	if (a->type->tag != Ral_TYPETAG_BASE)
	{
		// Can't negative
		return NULL;
	}

	if (Ral_ISBASETYPE(Ral_BASETYPE_INT, a))
	{
		Ral_Object_Int* intobj = Ral_CopyIntObject((Ral_Object_Int*)a);
		intobj->value = -intobj->value;
	}
	else if (Ral_ISBASETYPE(Ral_BASETYPE_FLOAT, a))
	{
		Ral_Object_Float* floatobj = Ral_CopyFloatObject((Ral_Object_Float*)a);
		floatobj->value = -floatobj->value;
	} else
	{
		// Can't negative
		return NULL;
	}
}



Ral_Object* Ral_Object_Equality(
	Ral_Object* const a,
	Ral_Object* const b
)
{

	return NULL;
}



Ral_Object* Ral_Object_Inequality(
	Ral_Object* const a,
	Ral_Object* const b
)
{
	return NULL;
}

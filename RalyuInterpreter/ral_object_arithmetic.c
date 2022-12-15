#include "ral_object.h"

#include "ral_cli.h"



Ral_Bool Ral_Object_Assign(Ral_Object* const assign, Ral_Object* const value)
{
	if (!(assign && value)) return NULL;

	if (assign->type != value->type)
	{
		RalCLI_ERROR("Type mismatch!");
		return Ral_FALSE;
	}

	if (assign->type->tag == Ral_TYPETAG_BASE)
	{
		if (assign->type == Ral_TYPEINT)
		{
			Ral_Object_Int* intassign = (Ral_Object_Int*)assign;
			Ral_Object_Int* intvalue = (Ral_Object_Int*)value;
			intassign->value = intvalue->value;
			return Ral_TRUE;
		}
	}

	return Ral_FALSE;
}



Ral_Object* Ral_Object_Add(
	Ral_Object* const a,
	Ral_Object* const b
)
{
	if (!(a && b)) return NULL;

	if (a->type->tag != Ral_TYPETAG_BASE ||
		b->type->tag != Ral_TYPETAG_BASE)
	{
		// Must be base types
		RalCLI_ERROR("Cannot perform arithmetic on non base types!");
		return NULL;
	}

	if (a->type == Ral_TYPEINT)
	{
		Ral_Object_Int* int_a = (Ral_Object_Int*)a;
		
		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateIntObjectFromInt(int_a->value + int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat((float)int_a->value + float_b->value);
		}
		else
			return NULL;
	}

	else if (a->type == Ral_TYPEFLOAT)
	{
		Ral_Object_Float* float_a = (Ral_Object_Float*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value + (float)int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value + float_b->value);
		}
		else
			return NULL;
	}
	else
		return NULL;
}



Ral_Object* Ral_Object_Subtract(
	Ral_Object* const a,
	Ral_Object* const b
)
{
	if (!(a && b)) return NULL;

	if (a->type->tag != Ral_TYPETAG_BASE ||
		b->type->tag != Ral_TYPETAG_BASE)
	{
		// Must be base types
		RalCLI_ERROR("Cannot perform arithmetic on non base types!");
		return NULL;
	}

	if (a->type == Ral_TYPEINT)
	{
		Ral_Object_Int* int_a = (Ral_Object_Int*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateIntObjectFromInt(int_a->value - int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat((float)int_a->value - float_b->value);
		}
		else
			return NULL;
	}

	else if (a->type == Ral_TYPEFLOAT)
	{
		Ral_Object_Float* float_a = (Ral_Object_Float*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value - (float)int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value - float_b->value);
		}
		else
			return NULL;
	}
	else
		return NULL;
}



Ral_Object* Ral_Object_Multiply(
	Ral_Object* const a,
	Ral_Object* const b
)
{
	if (!(a && b)) return NULL;

	if (a->type->tag != Ral_TYPETAG_BASE ||
		b->type->tag != Ral_TYPETAG_BASE)
	{
		// Must be base types
		RalCLI_ERROR("Cannot perform arithmetic on non base types!");
		return NULL;
	}

	if (a->type == Ral_TYPEINT)
	{
		Ral_Object_Int* int_a = (Ral_Object_Int*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateIntObjectFromInt(int_a->value * int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat((float)int_a->value * float_b->value);
		}
		else
			return NULL;
	}

	else if (a->type == Ral_TYPEFLOAT)
	{
		Ral_Object_Float* float_a = (Ral_Object_Float*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value * (float)int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value * float_b->value);
		}
		else
			return NULL;
	}
	else
		return NULL;
}



Ral_Object* Ral_Object_Divide(
	Ral_Object* const a,
	Ral_Object* const b
)
{
	if (!(a && b)) return NULL;

	if (a->type->tag != Ral_TYPETAG_BASE ||
		b->type->tag != Ral_TYPETAG_BASE)
	{
		// Must be base types
		RalCLI_ERROR("Cannot perform arithmetic on non base types!");
		return NULL;
	}

	if (a->type == Ral_TYPEINT)
	{
		Ral_Object_Int* int_a = (Ral_Object_Int*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateIntObjectFromInt(int_a->value / int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat((float)int_a->value / float_b->value);
		}
		else
			return NULL;
	}

	else if (a->type == Ral_TYPEFLOAT)
	{
		Ral_Object_Float* float_a = (Ral_Object_Float*)a;

		if (b->type == Ral_TYPEINT)
		{
			Ral_Object_Int* int_b = (Ral_Object_Int*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value / (float)int_b->value);
		} 
		else if (b->type == Ral_TYPEFLOAT)
		{
			Ral_Object_Float* float_b = (Ral_Object_Float*)b;
			return Ral_CreateFloatObjectFromFloat(float_a->value / float_b->value);
		}
		else
			return NULL;
	}
	else
		return NULL;
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

	if (a->type == Ral_TYPEINT)
	{
		Ral_Object_Int* intobj = Ral_CopyIntObject((Ral_Object_Int*)a);
		intobj->value = -intobj->value;
		return intobj;
	}
	else if (a->type == Ral_TYPEFLOAT)
	{
		Ral_Object_Float* floatobj = Ral_CopyFloatObject((Ral_Object_Float*)a);
		floatobj->value = -floatobj->value;
		return floatobj;
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

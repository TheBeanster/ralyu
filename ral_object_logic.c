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





Ral_Object* Ral_ObjectAssigmentOperator(
	Ral_State* const state,
	const Ral_OperatorID op,
	Ral_Object** assignobj,
	const Ral_Object* const valueobj
)
{
	if (!(assignobj && valueobj)) return NULL;

	switch (op)
	{
	case Ral_OPERATOR_ASSIGN:
		Ral_DestroyObject(*assignobj);
		Ral_Object* newobj = Ral_CopyObject(valueobj);
		*assignobj = newobj;
		return Ral_CopyObject(valueobj);

	default:
	{
		// Horrific hack with the operator id, kind of like ASCII shifting letter case
		Ral_Object* result = Ral_ObjectArithmeticOperator(state, op + Ral_OPERATOR_ADDITION - 1, *assignobj, valueobj);
		if (!result)
		{
			Ral_DestroyObject(*assignobj);
			*assignobj = NULL;
			return NULL;
		}
		*assignobj = Ral_CopyObject(result);
		return result;
	}
	}

	return NULL;
}





static Ral_Number object_to_number(const Ral_Object* const obj)
{
	if (obj->type == Ral_TYPE_BOOL) return obj->val.boolean;
	return obj->val.number;
}



Ral_Object* Ral_ObjectArithmeticOperator(
	Ral_State* const state,
	const enum Ral_OperatorID op,
	const Ral_Object* const a,
	const Ral_Object* const b
)
{
	if (a->type == Ral_TYPE_STRING || b->type == Ral_TYPE_STRING)
	{
		// If either object is a string then result will also be a string

		return 0xABCDEF69;
	}

	if (a->type == Ral_TYPE_ARRAY || a->type == Ral_TYPE_STRUCT ||
		b->type == Ral_TYPE_ARRAY || b->type == Ral_TYPE_STRUCT)
	{

		state->errormsg = "Cannot perform arithmetic operator on struct or array";
		return NULL;
	}
	
	// The only other types are bool and number
	Ral_Number numa = object_to_number(a);
	Ral_Number numb = object_to_number(b);

	switch (op)
	{
	case Ral_OPERATOR_ADDITION:			return Ral_CreateNumberObject(numa + numb);
	case Ral_OPERATOR_SUBTRACTION:		return Ral_CreateNumberObject(numa - numb);
	case Ral_OPERATOR_MULTIPLICATION:	return Ral_CreateNumberObject(numa * numb);
	case Ral_OPERATOR_DIVISION:			return Ral_CreateNumberObject(numa / numb);
	}

	state->errormsg = "Invalid arithmetic operator";
	return NULL;
}






static Ral_Bool struct_equals(const Ral_Object* const a, const Ral_Object* const b)
{
	Ral_StructMember* iterator = a->val.struc.members.begin;
	while (iterator)
	{
		// Member in b with same name as a
		Ral_StructMember* corresp_member = Ral_GetStructMemberNode(b, iterator->name);
		if (!corresp_member)
			return Ral_FALSE;

		if (!object_equals(iterator->obj, corresp_member->obj)) return Ral_FALSE;

		iterator = iterator->next;
	}

	return Ral_TRUE;
}

static Ral_Bool object_equals(const Ral_Object* const a, const Ral_Object* const b)
{
	switch (a->type)
	{
	case Ral_TYPE_BOOL:
		return a->val.boolean == a->val.boolean;

	case Ral_TYPE_NUMBER:
		return a->val.number == b->val.number;

	case Ral_TYPE_STRING:
		if (a->val.string.length != b->val.string.length) return Ral_FALSE;
		return strcmp(a->val.string.chars, b->val.string.chars) == 0;

	case Ral_TYPE_ARRAY:
		return Ral_FALSE;

	case Ral_TYPE_STRUCT:
		return struct_equals(a, b);
		break;

	default:
		break;
	}
}



Ral_Bool Ral_ObjectRelationalOperator(
	Ral_State* const state,
	const enum Ral_OperatorID op,
	const Ral_Object* const a,
	const Ral_Object* const b)
{
	if (a->type != b->type) return Ral_FALSE;

	switch (op)
	{
	case Ral_OPERATOR_EQUALITY:
		return object_equals(a, b);

	case Ral_OPERATOR_INEQUALITY:
		return !object_equals(a, b);

	default:
		// Relational operator is < > <= or >=
		if (a->type != Ral_TYPE_NUMBER) // Must be number
		{
			state->errormsg = "Operators < > <= and >= only work on numbers";
			return Ral_FALSE;
		}

		switch (op)
		{
		case Ral_OPERATOR_LESS: return a->val.number < b->val.number; break;
		case Ral_OPERATOR_GREATER: return a->val.number > b->val.number; break;
		case Ral_OPERATOR_LESSOREQUAL: return a->val.number <= b->val.number; break;
		case Ral_OPERATOR_GREATEROREQUAL: return a->val.number >= b->val.number; break;
		}
	}
	return Ral_FALSE;
}


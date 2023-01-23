#include "ral_stdlibrary.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "ralu_list.h"

#include "ral_object.h"
#include "ral_library.h"



static Ral_Object* stdmath_sin(Ral_State* const state, Ral_List* const arguments)
{
	if (arguments->itemcount != 1) return NULL;

	Ral_Object* arg = arguments->begin;
	if (arg->type != Ral_TYPE_NUMBER) return NULL;
	
	return Ral_CreateNumberObject(sin(arg->val.number));
}



static Ral_Object* stdmath_cos(Ral_State* const state, Ral_List* const arguments)
{
	if (arguments->itemcount != 1) return NULL;

	Ral_Object* arg = arguments->begin;
	if (arg->type != Ral_TYPE_NUMBER) return NULL;

	return Ral_CreateNumberObject(cos(arg->val.number));
}



static Ral_Object* stdmath_tan(Ral_State* const state, Ral_List* const arguments)
{
	if (arguments->itemcount != 1) return NULL;

	Ral_Object* arg = arguments->begin;
	if (arg->type != Ral_TYPE_NUMBER) return NULL;

	return Ral_CreateNumberObject(tan(arg->val.number));
}





Ral_Library(stdmath,
	Ral_LibFunction(stdmath_, sin)
	Ral_LibFunction(stdmath_, cos)
	Ral_LibFunction(stdmath_, tan)
	Ral_LibNumber("PI", M_PI)
	Ral_LibNumber("TAU", M_PI * 2)
	Ral_LibNumber("E", M_E)
);



Ral_LoadLib_STDMath(Ral_State* const state)
{
	Ral_LoadLib(state, stdmath);
}
#include "ral_stdlibrary.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "ralu_list.h"

#include "ral_object.h"
#include "ral_library.h"



#define MATHFUNC_1ARG(base_c_function)														\
	(Ral_State* const state, Ral_List* const arguments)										\
	{																						\
		if (arguments->itemcount != 1) return NULL;											\
																							\
		Ral_Object* arg = arguments->begin;													\
		if (!arg) return NULL;																\
		if (arg->type != Ral_TYPE_NUMBER) return NULL;										\
																							\
		return Ral_CreateNumberObject(base_c_function(arg->val.number));					\
	}



#define MATHFUNC_2ARGS(base_c_function)														\
	(Ral_State* const state, Ral_List* const arguments)										\
	{																						\
		if (arguments->itemcount != 2) return NULL;											\
																							\
		Ral_Object* arg1 = arguments->begin;												\
		if (!arg1) return NULL;																\
		if (arg1->type != Ral_TYPE_NUMBER) return NULL;										\
		Ral_Object* arg2 = arguments->begin->next;											\
		if (!arg2) return NULL;																\
		if (arg2->type != Ral_TYPE_NUMBER) return NULL;										\
																							\
		return Ral_CreateNumberObject(base_c_function(arg1->val.number, arg2->val.number));	\
	}



static Ral_Object* stdmath_sin		MATHFUNC_1ARG(sin);
static Ral_Object* stdmath_cos		MATHFUNC_1ARG(cos);
static Ral_Object* stdmath_tan		MATHFUNC_1ARG(tan);
static Ral_Object* stdmath_sinh		MATHFUNC_1ARG(sinh);
static Ral_Object* stdmath_cosh		MATHFUNC_1ARG(cosh);
static Ral_Object* stdmath_tanh		MATHFUNC_1ARG(tanh);
static Ral_Object* stdmath_asin		MATHFUNC_1ARG(asin);
static Ral_Object* stdmath_acos		MATHFUNC_1ARG(acos);
static Ral_Object* stdmath_atan		MATHFUNC_1ARG(atan);
static Ral_Object* stdmath_asinh	MATHFUNC_1ARG(asinh);
static Ral_Object* stdmath_acosh	MATHFUNC_1ARG(acosh);
static Ral_Object* stdmath_atanh	MATHFUNC_1ARG(atanh);

static Ral_Object* stdmath_exp		MATHFUNC_1ARG(exp);
static Ral_Object* stdmath_ln		MATHFUNC_1ARG(log);
static Ral_Object* stdmath_log		MATHFUNC_1ARG(log10);
static Ral_Object* stdmath_log2		MATHFUNC_1ARG(log2);
static Ral_Object* stdmath_sqrt		MATHFUNC_1ARG(sqrt);
static Ral_Object* stdmath_cbrt		MATHFUNC_1ARG(cbrt);
static Ral_Object* stdmath_round	MATHFUNC_1ARG(round);
static Ral_Object* stdmath_floor	MATHFUNC_1ARG(floor);
static Ral_Object* stdmath_ceil		MATHFUNC_1ARG(ceil);
static Ral_Object* stdmath_abs		MATHFUNC_1ARG(fabs);
static Ral_Object* stdmath_trunc	MATHFUNC_1ARG(trunc);

static Ral_Object* stdmath_atan2	MATHFUNC_2ARGS(atan2);

static Ral_Object* stdmath_pow		MATHFUNC_2ARGS(pow);
static Ral_Object* stdmath_mod		MATHFUNC_2ARGS(fmod);
static Ral_Object* stdmath_hypot	MATHFUNC_2ARGS(hypot);



static Ral_Object* stdmath_random(Ral_State* const state, Ral_List* const arguments)
{
	if (arguments->itemcount == 0)
	{
		// Get number from 0 to 1
		Ral_Number r = (Ral_Number)rand() / RAND_MAX;
		return Ral_CreateNumberObject(r);
	} else if (arguments->itemcount == 1)
	{
		// Get number from 0 to arg
		Ral_Object* arg = arguments->begin;
		if (!arg) return NULL;
		if (arg->type != Ral_TYPE_NUMBER) return NULL;
		Ral_Number r = ((Ral_Number)rand() / RAND_MAX) * arg->val.number;
		return Ral_CreateNumberObject(r);
	} else if (arguments->itemcount == 2)
	{
		// Get number from arg1 to arg2
		Ral_Object* arg1 = arguments->begin;
		if (!arg1) return NULL;
		if (arg1->type != Ral_TYPE_NUMBER) return NULL;
		Ral_Object* arg2 = arguments->begin->next;
		if (!arg2) return NULL;
		if (arg2->type != Ral_TYPE_NUMBER) return NULL;
		Ral_Number r = ((Ral_Number)rand() / RAND_MAX) * (arg2->val.number - arg1->val.number) + arg1->val.number;
		return Ral_CreateNumberObject(r);
	} else
	{
		// Too many args
		return NULL;
	}
}



Ral_Library(stdmath,
	Ral_LibFunction(stdmath_, sin)
	Ral_LibFunction(stdmath_, cos)
	Ral_LibFunction(stdmath_, tan)
	Ral_LibFunction(stdmath_, sinh)
	Ral_LibFunction(stdmath_, cosh)
	Ral_LibFunction(stdmath_, tanh)
	Ral_LibFunction(stdmath_, asin)
	Ral_LibFunction(stdmath_, acos)
	Ral_LibFunction(stdmath_, atan)
	Ral_LibFunction(stdmath_, asinh)
	Ral_LibFunction(stdmath_, acosh)
	Ral_LibFunction(stdmath_, atanh)

	Ral_LibFunction(stdmath_, exp)
	Ral_LibFunction(stdmath_, ln)
	Ral_LibFunction(stdmath_, log)
	Ral_LibFunction(stdmath_, log2)
	Ral_LibFunction(stdmath_, sqrt)
	Ral_LibFunction(stdmath_, cbrt)
	Ral_LibFunction(stdmath_, round)
	Ral_LibFunction(stdmath_, floor)
	Ral_LibFunction(stdmath_, ceil)
	Ral_LibFunction(stdmath_, abs)
	Ral_LibFunction(stdmath_, trunc)

	Ral_LibFunction(stdmath_, atan2)

	Ral_LibFunction(stdmath_, pow)
	Ral_LibFunction(stdmath_, mod)
	Ral_LibFunction(stdmath_, hypot)

	Ral_LibFunction(stdmath_, random)

	Ral_LibNumber("PI", M_PI)
	Ral_LibNumber("TAU", M_PI * 2)
	Ral_LibNumber("E", M_E)
);



Ral_LoadLib_STDMath(Ral_State* const state)
{
	Ral_LoadLib(state, stdmath);
}
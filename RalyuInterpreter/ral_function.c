#include "ral_function.h"

#include "ral_logging.h"
#include "ral_cli.h"



static Ral_Bool validate_parameters(
	const Ral_Function* const function,
	const Ral_List* const parameters
)
{
	Ral_Object* callparam = parameters->begin;
	Ral_Object* funcparam = function->parameters.begin;
	while (1)
	{
		// If function parameter type is NULL then any type can be passed
		if (funcparam->type)
		{
			if (funcparam->type != callparam->type)
			{
				RalCLI_ERROR("RUNTIME ERROR : Function parameter mismatch!");
				return Ral_FALSE;
			}
		}

		callparam = callparam->next;
		if ((callparam == NULL) && (funcparam != NULL))
		{
			// Ran out of call parameters
			RalCLI_ERROR("RUNTIME ERROR : Too few parameters for function call!");
			return Ral_FALSE;
		}
		funcparam = funcparam->next;
		if ((funcparam == NULL) && (callparam != NULL))
		{
			// Ran out of call parameters
			RalCLI_ERROR("RUNTIME ERROR : Too many parameters for function call!");
			return Ral_FALSE;
		}
	}
}



Ral_Object* Ral_CallFunction(
	Ral_State* const state,
	const Ral_Function* const function,
	const Ral_List* const parameters
)
{
	if (!validate_parameters(function, parameters)) return NULL;

	if (function->extern_function != NULL)
	{
		// If there is an extern function pointer run it instead
		Ral_Object* return_object = (function->extern_function)(parameters);
	} else
	{
		// Run the code

		Ral_List local_variables = { 0 };
		Ral_Statement* current_statement = function->declaration;
		while (current_statement)
		{
			current_statement = Ral_ExecuteStatement(state, current_statement);
		}
		Ral_ClearList(&local_variables, &Ral_DestroyObject);
	}
}

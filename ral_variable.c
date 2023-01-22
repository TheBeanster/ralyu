#include "ral_variable.h"

#include "ralu_memory.h"



Ral_Variable* Ral_CreateVariable(const char* const name, Ral_Object* const value)
{
	Ral_Variable* var = Ral_ALLOC_TYPE(Ral_Variable);
	var->name = _strdup(name);
	var->obj = value;
	return var;
}



Ral_Variable* Ral_DeclareVariable(Ral_List* const variable_list, Ral_Variable* const variable)
{
	if (!variable) return NULL;

	Ral_PushBackList(variable_list, variable);

	return variable;
}



Ral_Variable* Ral_GetVariable(
	Ral_State* const state,
	Ral_List* const local_variables,
	const char* const name
)
{
	Ral_Variable* iterator = local_variables->begin;
	while (iterator)
	{
		if (strcmp(iterator->name, name) == 0)
			return iterator;
		iterator = iterator->next;
	}

	iterator = state->global_variables.begin;
	while (iterator)
	{
		if (strcmp(iterator->name, name) == 0)
			return iterator;
		iterator = iterator->next;
	}

	Ral_Variable* var = Ral_CreateVariable(name, Ral_CreateNumberObject(0));
	if (local_variables)
		return Ral_DeclareVariable(local_variables, var);
	else
		return Ral_DeclareVariable(&state->global_variables, var);
}
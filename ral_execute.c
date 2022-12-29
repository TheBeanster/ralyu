#include "ral_execute.h"

#include "ral_logging.h"
#include "ral_object.h"



Ral_Object* Ral_ExecuteGlobalSourceUnit(
	Ral_State* const state,
	const Ral_SourceUnit* const source
)
{
	if (!state) return;
	if (!source) return;
	if (source->numstatements <= 0) return;
	if (!source->statements) return;

	Ral_Statement* current_statement = &source->statements[0];
	Ral_Object* return_object = NULL;

	while (current_statement)
	{
		current_statement = Ral_ExecuteStatement(
			state,
			current_statement,
			&state->global_variables,
			Ral_TRUE,
			&return_object
		);
	}

	return return_object;
}



Ral_Statement* Ral_ExecuteStatement(
	Ral_State* const state,
	const Ral_Statement* const statement,
	Ral_List* const scope_variables,
	const Ral_Bool global,
	Ral_Object** const return_object
)
{
	printf(" - Executing statement -\n");

	switch (statement->type)
	{



	case Ral_STATEMENTTYPE_EXPRESSION:
	{
		Ral_Token* first_token = &statement->tokens[0];

		Ral_Type* first_token_type = Ral_GetTypeS(state, first_token->string);
		if (first_token_type)
		{
			// Expression begins with a type
			// Expression is a declaration

			int identifierid = 0;
			for (int i = 1; i < statement->numtokens; i++)
			{
				if (statement->tokens[i].type == Ral_TOKENTYPE_IDENTIFIER)
				{
					identifierid = i;
					break;
				}
			}
			Ral_Type* type = Ral_GetTypeT(state, statement->tokens, 0, identifierid);
			if (!type) break;

			Ral_Object* var = Ral_DeclareVariable(scope_variables, type, statement->tokens[identifierid].string);

			*return_object = var;
		} else
		{

		}
	}
		break;



	default:
		printf("Invalid statement type\n");
		return NULL;
		break;
	}

	if (statement->index + 1 >= statement->parentsource->numstatements) return NULL; // Reached end of sourceunit
	return &statement->parentsource->statements[statement->index + 1];
}

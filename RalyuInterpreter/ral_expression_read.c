#include "ral_expression.h"

#include "ralu_memory.h"



Ral_Expression* Ral_CreateExpression(
	const Ral_Statement* const statement,
	const int begin,
	const int end
)
{
	// Validate
	if (!statement) return NULL;
	if (begin < 0) return NULL;
	if (end <= begin) return NULL;
	if (end > statement->numtokens) return NULL;

	Ral_Expression* expression = Ral_ALLOC_TYPE(Ral_Expression);

	int current_paren_depth = 0;
	for (int i = begin; i < end; i++)
	{
		Ral_Token* token = &statement->tokens[i];
		
		if (token->type == Ral_TOKENTYPE_OPERATOR)
		{

		}
	}

	return expression;
}





void Ral_DestroyExpression(
	Ral_Expression* const expression
)
{
	// TODO Implement
}

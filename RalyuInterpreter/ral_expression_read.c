#include "ral_expression.h"

#include "ral_memory.h"



Ral_Expression* Ral_BuildExpressionTree(
	const Ral_Statement* const statement,
	const int being,
	const int end
)
{
	Ral_Expression* expression = Ral_ALLOC_TYPE(Ral_Expression);

	Ral_Token* token;
	for (int i = 0; i < statement->numtokens; i++)
	{
		token = &statement->tokens[i];

		
	}

	return expression;
}
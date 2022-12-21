#include "ral_lexer.h"

#include <stdio.h>
#include "ralu_memory.h"

#include "ral_sourceunit.h"



const char* const ral_serialized_statementtype_names[] =
{
	Ral_STR_PREFIX "STATEMENTTYPE_NULL",
	Ral_STR_PREFIX "STATEMENTTYPE_EXPRESSION",
	Ral_STR_PREFIX "STATEMENTTYPE_STRUCT",
	Ral_STR_PREFIX "STATEMENTTYPE_FUNCTION",
	Ral_STR_PREFIX "STATEMENTTYPE_RETURN",
	Ral_STR_PREFIX "STATEMENTTYPE_IF",
	Ral_STR_PREFIX "STATEMENTTYPE_ELSE",
	Ral_STR_PREFIX "STATEMENTTYPE_FOR",
	Ral_STR_PREFIX "STATEMENTTYPE_WHILE",
	Ral_STR_PREFIX "STATEMENTTYPE_GOTO",
	Ral_STR_PREFIX "STATEMENTTYPE_END"
};



Ral_Statement* Ral_CreateStatement(
	const Ral_Token* const		begin,
	const Ral_Token* const		end,
	const Ral_StatementType		type
)
{
	// Count the number of tokens in the statement and make sure the tokens enclose a list
	int numtokens = 0;
	Ral_Token* iterator = begin;
	while (1)
	{
		if (iterator->type != Ral_TOKENTYPE_ENDLINE)
			numtokens++;
		if (iterator == end) break;
		iterator = iterator->next;
		if (!iterator)
			return NULL;
	}

	Ral_Statement* statement = Ral_ALLOC_TYPE(Ral_Statement);
	statement->numtokens = numtokens;
	statement->tokens = Ral_CALLOC(numtokens, sizeof(Ral_Token));
	iterator = begin;
	for (int i = 0; i < numtokens; i++)
	{
		if (iterator->type == Ral_TOKENTYPE_ENDLINE) // Skip endline tokens
		{
			iterator = iterator->next;
			i--; // Step back to not skip anything in the array
			continue;
		}
		statement->tokens[i] = *iterator;
		iterator = iterator->next;
	}

	statement->type = type;
	statement->parentsource = NULL;

	return statement;
}





void Ral_DestroyStatement(Ral_Statement* const statement)
{
	for (int i = 0; i < statement->numtokens; i++)
	{
		Ral_DestroyToken(&statement->tokens[i]);
	}
	Ral_FREE(statement);
}





void Ral_PrintStatementString(const Ral_Statement* const statement)
{
	if (!statement) return;
	if (!statement->tokens) return;

	for (int i = 0; i < statement->numtokens; i++)
	{
		if (statement->tokens[i].type == Ral_TOKENTYPE_ENDLINE)
			printf("\\n ");
		else
			printf("%s ", statement->tokens[i].string);
	}
}



void Ral_PrintStatementTokens(const Ral_Statement* const statement)
{
	Ral_PrintStatementString(statement);
	printf("\n%s\n    ", ral_serialized_statementtype_names[statement->type]);
	for (int i = 0; i < statement->numtokens; i++)
	{
		Ral_PrintToken(&statement->tokens[i]);
		printf("\n    ");
	}
	putchar('\n');
}
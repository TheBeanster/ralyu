#include "ral_lexer.h"

#include <stdio.h>
#include <string.h>
#include "ralu_string.h"
#include "ralu_memory.h"



Ral_Token* Ral_CreateToken(
	const char* const	string,
	const int			start,
	const int			end,
	const int			linenum,
	const int			type
)
{
	Ral_Token* token = Ral_ALLOC_TYPE(Ral_Token);
	token->string = Ral_CreateCutString(string, start, end - start);
	token->position = start;
	token->linenum = linenum;
	token->type = type;
	token->keywordid = Ral_NOT_KEYWORD;
	token->operatorid = Ral_NOT_OPERATOR;
	token->separatorid = Ral_NOT_SEPARATOR;
	return token;
}



void Ral_DestroyToken(
	Ral_Token* const token
)
{
	if (token->string)
		Ral_FREE(token->string);
	Ral_FREE(token);
}



void Ral_PrintToken(const Ral_Token* const token)
{
	if (token->type != Ral_TOKENTYPE_ENDLINE)
		printf("Token \"%s\"\x1B[30G| type = ", token->string);
	else
		printf("Token \"\\n\"\x1B[30G| type = ");

	switch (token->type)
	{
	case Ral_TOKENTYPE_KEYWORD:
		printf("%s", ral_serialized_keywordid_names[token->keywordid]);
		break;

	case Ral_TOKENTYPE_IDENTIFIER:
		printf(Ral_STR_PREFIX "TOKENTYPE_IDENTIFIER");
		break;

	case Ral_TOKENTYPE_INTLITERAL:
		printf(Ral_STR_PREFIX "TOKENTYPE_INTLITERAL");
		break;

	case Ral_TOKENTYPE_FLOATLITERAL:
		printf(Ral_STR_PREFIX "TOKENTYPE_FLOATLITERAL");
		break;

	case Ral_TOKENTYPE_STRINGLITERAL:
		printf(Ral_STR_PREFIX "TOKENTYPE_STRINGLITERAL");
		break;

	case Ral_TOKENTYPE_OPERATOR:
		printf("%s", ral_serialized_operatorid_names[token->operatorid]);
		break;

	case Ral_TOKENTYPE_SEPARATOR:
		printf("%s", ral_serialized_separatorid_names[token->separatorid]);
		break;

	case Ral_TOKENTYPE_ENDLINE:
		printf(Ral_STR_PREFIX "TOKENTYPE_ENDLINE");
		break;

	default:
		printf("Invalid");
		break;
	}
}

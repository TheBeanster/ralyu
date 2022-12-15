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
	token->numberliteral_value = 0.0;
	token->stringliteral_value = NULL;
	return token;
}



void Ral_DestroyToken(
	Ral_Token* const token
)
{
	Ral_FREE(token->string);
	if (token->stringliteral_value)
		Ral_FREE(token->stringliteral_value);
	Ral_FREE(token);
}



void Ral_PrintToken(const Ral_Token* const token)
{
	printf("Token \"%s\"\x1B[30G| type = ", token->string);
	switch (token->type)
	{
	case Ral_TOKENTYPE_KEYWORD:
		printf("%s", ral_keywordid_names[token->keywordid]);
		break;

	case Ral_TOKENTYPE_IDENTIFIER:
		printf(RALPREFIX "TOKENTYPE_IDENTIFIER");
		break;

	case Ral_TOKENTYPE_NUMBERLITERAL:
		printf(RALPREFIX "TOKENTYPE_NUMBERLITERAL");
		break;

	case Ral_TOKENTYPE_STRINGLITERAL:
		printf(RALPREFIX "TOKENTYPE_STRINGLITERAL");
		break;

	case Ral_TOKENTYPE_OPERATOR:
		printf("%s", ral_operatorid_names[token->operatorid]);
		break;

	case Ral_TOKENTYPE_SEPARATOR:
		printf("%s", ral_separatorid_names[token->separatorid]);
		break;

	default:
		printf("Invalid");
		break;
	}
}



void Ral_PrintTokenValue(const Ral_Token* const token)
{
	switch (token->type)
	{
	case Ral_TOKENTYPE_KEYWORD:
		printf("%s", ral_keywordid_names[token->keywordid]);
		break;

	case Ral_TOKENTYPE_IDENTIFIER:
		printf("%s", token->string);
		break;

	case Ral_TOKENTYPE_NUMBERLITERAL:
		printf("%f", token->numberliteral_value);
		break;

	case Ral_TOKENTYPE_STRINGLITERAL:
		putchar('\"');
		Ral_PrintCondensedString(token->stringliteral_value);
		putchar('\"');
		break;

	case Ral_TOKENTYPE_OPERATOR:
		printf("%s", ral_operatorid_names[token->operatorid]);
		break;

	case Ral_TOKENTYPE_SEPARATOR:
		printf("%s", ral_separatorid_names[token->separatorid]);
		break;

	default:
		printf("Invalid");
		break;
	}
}

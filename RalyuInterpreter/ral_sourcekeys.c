#include "ral_lexer.h"

#include <string.h>



const char* ral_keyword_strings[] =
{
	"return",

	"if",
	"then",
	"else",

	"for",
	"while",
	"do",
	"goto",

	"end",

	"print",

	"void",
	"bool",
	"int",
	"float",
	"char",
	"string",

	"false",
	"true",

	"function"
};

Ral_KeywordID Ral_CheckKeyword(const char* const str)
{
	for (int i = 0; i < Ral_NUM_KEYWORDS; i++)
	{
		if (!strcmp(str, ral_keyword_strings[i]))
		{
			return i;
		}
	}
	return Ral_NOT_KEYWORD;
}

const char* ral_keywordid_names[] =
{
	"Ral_KEYWORD_RETURN",

	"Ral_KEYWORD_IF",
	"Ral_KEYWORD_THEN",
	"Ral_KEYWORD_ELSE",

	"Ral_KEYWORD_FOR",
	"Ral_KEYWORD_WHILE",
	"Ral_KEYWORD_DO",
	"Ral_KEYWORD_GOTO",

	"Ral_KEYWORD_END",

	"Ral_KEYWORD_PRINT",

	"Ral_KEYWORD_VOID",
	"Ral_KEYWORD_BOOL",
	"Ral_KEYWORD_INT",
	"Ral_KEYWORD_FLOAT",
	"Ral_KEYWORD_CHAR",
	"Ral_KEYWORD_STRING",

	"Ral_KEYWORD_FALSE",
	"Ral_KEYWORD_TRUE",

	"Ral_KEYWORD_FUNCTION"
};





const char* ral_operator_strings[] =
{
	"=",
	"+=",
	"-=",
	"*=",
	"/=",

	"!",
	"", // Unary negative should not be set by finding it here

	"==",
	"!=",
	"<",
	">",
	"<=",
	">=",

	"+",
	"-",
	"*",
	"/",
	"%",

	"."
};

Ral_OperatorID Ral_CheckOperator(const char* const str)
{
	for (int i = 0; i < Ral_NUM_OPERATORS; i++)
	{
		if (!strcmp(str, ral_operator_strings[i]))
		{
			return i;
		}
	}
	return Ral_NOT_OPERATOR;
}

const char* ral_operatorid_names[] =
{
	"Ral_OPERATOR_ASSIGN",
	"Ral_OPERATOR_ASSIGNADDITION",
	"Ral_OPERATOR_ASSIGNSUBTRACTION",
	"Ral_OPERATOR_ASSIGNMULTIPLICATION",
	"Ral_OPERATOR_ASSIGNDIVISION",

	"Ral_OPERATOR_NOT",
	"Ral_OPERATOR_NEGATIVE",

	"Ral_OPERATOR_EQUALITY",
	"Ral_OPERATOR_INEQUALITY",
	"Ral_OPERATOR_LESS",
	"Ral_OPERATOR_GREATER",
	"Ral_OPERATOR_LESSOREQUAL",
	"Ral_OPERATOR_GREATEROREQUAL",

	"Ral_OPERATOR_ADDITION",
	"Ral_OPERATOR_SUBTRACTION",
	"Ral_OPERATOR_MULTIPLICATION",
	"Ral_OPERATOR_DIVISION",
	"Ral_OPERATOR_MODULUS",

	"Ral_OPERATOR_MEMBERACCESS"
};

const int operator_precedence[] =
{
	1, // "ASSIGN",
	1, // "ASSIGNADDITION",
	1, // "ASSIGNSUBTRACTION",
	1, // "ASSIGNMULTIPLICATION",
	1, // "ASSIGNDIVISION",

	8, // "NOT",
	7, // "NEGATIVE"

	2, // "EQUALITY",
	2, // "INEQUALITY",
	3, // "LESS",
	3, // "GREATER",
	3, // "LESSOREQUAL",
	3, // "GREATEROREQUAL",

	4, // "ADDITION",
	4, // "SUBTRACTION",
	5, // "MULTIPLICATION",
	5, // "DIVISION",
	5, // "MODULUS",

	9, // "MEMBERACCESS"
};





const char ral_separator_chars[] =
{
	',',
	'(',
	')',
	":"
};

Ral_OperatorID Ral_CheckSeparator(const char character)
{
	for (int i = 0; i < Ral_NUM_SEPARATORS; i++)
	{
		if (character == ral_separator_chars[i])
		{
			return i;
		}
	}
	return Ral_NOT_SEPARATOR;
}

const char* ral_separatorid_names[] =
{
	"Ral_SEPARATOR_COMMA",
	"Ral_SEPARATOR_LPAREN",
	"Ral_SEPARATOR_RPAREN",
	"Ral_SEPARATOR_COLON"
};



const char* const ral_statementtype_names[] =
{
	"Ral_STATEMENTTYPE_NULL",

	"Ral_STATEMENTTYPE_EXPRESSION",
	"Ral_STATEMENTTYPE_DECLARATION",
	"Ral_STATEMENTTYPE_FUNCTION",

	"Ral_STATEMENTTYPE_IF",
	"Ral_STATEMENTTYPE_ELSE",
	"Ral_STATEMENTTYPE_FOR",
	"Ral_STATEMENTTYPE_WHILE",
	"Ral_STATEMENTTYPE_GOTO",

	"Ral_STATEMENTTYPE_RETURN",
	"Ral_STATEMENTTYPE_END",

	"Ral_STATEMENTTYPE_PRINT",

	"Ral_NUM_STATEMENTTYPES",

	"Ral_STATREAD_DECLARATION"
};
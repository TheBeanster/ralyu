#include "ral_lexer.h"

#include <string.h>



const char* ral_keyword_strings[] =
{
	"return",

	"if",
	"else",

	"for",
	"while",
	"do",
	"goto",

	"print",

	"struct",

	"void",
	"bool",
	"int",
	"float",
	"char",
	"string",

	"false",
	"true",

	"var",

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
	RALPREFIX "KEYWORD_RETURN",

	RALPREFIX "KEYWORD_IF",
	RALPREFIX "KEYWORD_ELSE",

	RALPREFIX "KEYWORD_FOR",
	RALPREFIX "KEYWORD_WHILE",
	RALPREFIX "KEYWORD_DO",
	RALPREFIX "KEYWORD_GOTO",

	RALPREFIX "KEYWORD_PRINT",

	RALPREFIX "KEYWORD_STRUCT",

	RALPREFIX "KEYWORD_VOID",
	RALPREFIX "KEYWORD_BOOL",
	RALPREFIX "KEYWORD_INT",
	RALPREFIX "KEYWORD_FLOAT",
	RALPREFIX "KEYWORD_CHAR",
	RALPREFIX "KEYWORD_STRING",

	RALPREFIX "KEYWORD_FALSE",
	RALPREFIX "KEYWORD_TRUE",

	RALPREFIX "KEYWORD_VAR",

	RALPREFIX "KEYWORD_FUNCTION"
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
	RALPREFIX "OPERATOR_ASSIGN",
	RALPREFIX "OPERATOR_ASSIGNADDITION",
	RALPREFIX "OPERATOR_ASSIGNSUBTRACTION",
	RALPREFIX "OPERATOR_ASSIGNMULTIPLICATION",
	RALPREFIX "OPERATOR_ASSIGNDIVISION",

	RALPREFIX "OPERATOR_NOT",
	RALPREFIX "OPERATOR_NEGATIVE",

	RALPREFIX "OPERATOR_EQUALITY",
	RALPREFIX "OPERATOR_INEQUALITY",
	RALPREFIX "OPERATOR_LESS",
	RALPREFIX "OPERATOR_GREATER",
	RALPREFIX "OPERATOR_LESSOREQUAL",
	RALPREFIX "OPERATOR_GREATEROREQUAL",

	RALPREFIX "OPERATOR_ADDITION",
	RALPREFIX "OPERATOR_SUBTRACTION",
	RALPREFIX "OPERATOR_MULTIPLICATION",
	RALPREFIX "OPERATOR_DIVISION",
	RALPREFIX "OPERATOR_MODULUS",

	RALPREFIX "OPERATOR_MEMBERACCESS"
};

const int ral_operator_precedence[] =
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
	';',
	',',
	'(',
	')',
	'[',
	']',
	'{',
	'}',
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
	RALPREFIX "SEPARATOR_SEMICOLON",
	RALPREFIX "SEPARATOR_COMMA",
	RALPREFIX "SEPARATOR_LPAREN",
	RALPREFIX "SEPARATOR_RPAREN",
	RALPREFIX "SEPARATOR_LBRACKET",
	RALPREFIX "SEPARATOR_RBRACKET",
	RALPREFIX "SEPARATOR_LBRACE",
	RALPREFIX "SEPARATOR_RBRACE",
	RALPREFIX "SEPARATOR_COLON"
};



const char* const ral_statementtype_names[] =
{
	RALPREFIX "STATEMENTTYPE_NULL",

	RALPREFIX "STATEMENTTYPE_EXPRESSION",
	RALPREFIX "STATEMENTTYPE_DECLARATION",

	RALPREFIX "STATEMENTTYPE_STRUCT",
	RALPREFIX "STATEMENTTYPE_FUNCTION",
	RALPREFIX "STATEMENTTYPE_ENDBRACE",

	RALPREFIX "STATEMENTTYPE_IF",
	RALPREFIX "STATEMENTTYPE_ELSE",
	RALPREFIX "STATEMENTTYPE_FOR",
	RALPREFIX "STATEMENTTYPE_WHILE",
	RALPREFIX "STATEMENTTYPE_GOTO",

	RALPREFIX "STATEMENTTYPE_RETURN",
	RALPREFIX "STATEMENTTYPE_END",

	RALPREFIX "STATEMENTTYPE_PRINT",

	RALPREFIX "NUM_STATEMENTTYPES",

	RALPREFIX "STATREAD_DECLARATION"
};
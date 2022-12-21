#include "ral_lexer.h"

#include "ralu_string.h"



const char* const ral_keyword_strings[] =
{
	"if",
	"else",
	"for",
	"while",
	"do",
	"goto",
	"function",
	"return",
	"struct",
	"void",
	"bool",
	"int",
	"float",
	"string",
	"false",
	"true",
	"end"
};

Ral_KeywordID Ral_CheckKeyword(const char* const str)
{
	for (int i = 0; i < Ral_NUM_KEYWORDS; i++)
	{
		if (!strcmp(str, ral_keyword_strings[i]))
			return i;
	}
	return Ral_NOT_KEYWORD;
}

const char* const ral_serialized_keywordid_names[] =
{
	Ral_STR_PREFIX "KEYWORD_IF",
	Ral_STR_PREFIX "KEYWORD_ELSE",
	Ral_STR_PREFIX "KEYWORD_FOR",
	Ral_STR_PREFIX "KEYWORD_WHILE",
	Ral_STR_PREFIX "KEYWORD_DO",
	Ral_STR_PREFIX "KEYWORD_GOTO",
	Ral_STR_PREFIX "KEYWORD_FUNCTION",
	Ral_STR_PREFIX "KEYWORD_RETURN",
	Ral_STR_PREFIX "KEYWORD_STRUCT",
	Ral_STR_PREFIX "KEYWORD_VOID",
	Ral_STR_PREFIX "KEYWORD_BOOL",
	Ral_STR_PREFIX "KEYWORD_INT",
	Ral_STR_PREFIX "KEYWORD_FLOAT",
	Ral_STR_PREFIX "KEYWORD_STRING",
	Ral_STR_PREFIX "KEYWORD_FALSE",
	Ral_STR_PREFIX "KEYWORD_TRUE",
	Ral_STR_PREFIX "KEYWORD_END"
};





const char* const ral_operator_strings[] =
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
			return i;
	}
	return Ral_NOT_OPERATOR;
}

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

	9  // "MEMBERACCESS"
};

const char* const ral_serialized_operatorid_names[] =
{
	Ral_STR_PREFIX "OPERATOR_ASSIGN",
	Ral_STR_PREFIX "OPERATOR_ASSIGNADDITION",
	Ral_STR_PREFIX "OPERATOR_ASSIGNSUBTRACTION",
	Ral_STR_PREFIX "OPERATOR_ASSIGNMULTIPLICATION",
	Ral_STR_PREFIX "OPERATOR_ASSIGNDIVISION",

	Ral_STR_PREFIX "OPERATOR_NOT",
	Ral_STR_PREFIX "OPERATOR_NEGATIVE",

	Ral_STR_PREFIX "OPERATOR_EQUALITY",
	Ral_STR_PREFIX "OPERATOR_INEQUALITY",
	Ral_STR_PREFIX "OPERATOR_LESS",
	Ral_STR_PREFIX "OPERATOR_GREATER",
	Ral_STR_PREFIX "OPERATOR_LESSOREQUAL",
	Ral_STR_PREFIX "OPERATOR_GREATEROREQUAL",

	Ral_STR_PREFIX "OPERATOR_ADDITION",
	Ral_STR_PREFIX "OPERATOR_SUBTRACTION",
	Ral_STR_PREFIX "OPERATOR_MULTIPLICATION",
	Ral_STR_PREFIX "OPERATOR_DIVISION",
	Ral_STR_PREFIX "OPERATOR_MODULUS",

	Ral_STR_PREFIX "OPERATOR_MEMBERACCESS"
};





const char ral_separator_chars[] =
{
	',',
	':',
	'(',
	')',
	'[',
	']',
	'{',
	'}'
};

Ral_OperatorID Ral_CheckSeparator(const char character)
{
	for (int i = 0; i < Ral_NUM_SEPARATORS; i++)
	{
		if (character == ral_separator_chars[i])
			return i;
	}
	return Ral_NOT_SEPARATOR;
}

const char* const ral_serialized_separatorid_names[] =
{
	Ral_STR_PREFIX "SEPARATOR_COMMA",
	Ral_STR_PREFIX "SEPARATOR_COLON",
	Ral_STR_PREFIX "SEPARATOR_LPAREN",
	Ral_STR_PREFIX "SEPARATOR_RPAREN",
	Ral_STR_PREFIX "SEPARATOR_LBRACKET",
	Ral_STR_PREFIX "SEPARATOR_RBRACKET",
	Ral_STR_PREFIX "SEPARATOR_LBRACE",
	Ral_STR_PREFIX "SEPARATOR_RBRACE"
};

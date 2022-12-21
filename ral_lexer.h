#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"



// Include the Ral_ prefix in serialized enum names
#if 0
#define RALPREFIX "Ral_"
#else
#define Ral_STR_PREFIX ""
#endif



typedef enum
{
	Ral_KEYWORD_IF,
	Ral_KEYWORD_ELSE,
	Ral_KEYWORD_FOR,
	Ral_KEYWORD_WHILE,
	Ral_KEYWORD_DO,
	Ral_KEYWORD_GOTO,
	Ral_KEYWORD_FUNCTION,
	Ral_KEYWORD_RETURN,
	Ral_KEYWORD_STRUCT,
	Ral_KEYWORD_VOID,
	Ral_KEYWORD_BOOL,
	Ral_KEYWORD_INT,
	Ral_KEYWORD_FLOAT,
	Ral_KEYWORD_STRING,
	Ral_KEYWORD_FALSE,
	Ral_KEYWORD_TRUE,
	Ral_KEYWORD_END,

	Ral_NUM_KEYWORDS,
	Ral_NOT_KEYWORD = -1
} Ral_KeywordID;

extern const char* const ral_keyword_strings[];

Ral_KeywordID Ral_CheckKeyword(const char* const str);

extern const char* const ral_serialized_keywordid_names[];



typedef enum
{
	Ral_OPERATOR_ASSIGN,				// =
	Ral_OPERATOR_ASSIGNADDITION,		// +=
	Ral_OPERATOR_ASSIGNSUBTRACTION,		// -=
	Ral_OPERATOR_ASSIGNMULTIPLICATION,	// *=
	Ral_OPERATOR_ASSIGNDIVISION,		// /=

	Ral_OPERATOR_NOT,					// !
	Ral_OPERATOR_NEGATIVE,				// -

	Ral_OPERATOR_EQUALITY,				// ==
	Ral_OPERATOR_INEQUALITY,			// !=
	Ral_OPERATOR_LESS,					// <
	Ral_OPERATOR_GREATER,				// >
	Ral_OPERATOR_LESSOREQUAL,			// <=
	Ral_OPERATOR_GREATEROREQUAL,		// >=

	Ral_OPERATOR_ADDITION,				// +
	Ral_OPERATOR_SUBTRACTION,			// -
	Ral_OPERATOR_MULTIPLICATION,		// *
	Ral_OPERATOR_DIVISION,				// /

	Ral_OPERATOR_MEMBERACCESS,			// .

	Ral_NUM_OPERATORS,
	Ral_NOT_OPERATOR = -1
} Ral_OperatorID;

extern const char* const const ral_operator_strings[];

Ral_OperatorID Ral_CheckOperator(const char* const str);

#define Ral_IS_ASSIGNMENT_OPERATOR(op) ((op >= 0 && op <= 4) ? Ral_TRUE : Ral_FALSE)
#define Ral_IS_UNARY_OPERATOR(op) ((op >= 5 && op <= 7) ? Ral_TRUE : Ral_FALSE)

extern const int ral_operator_precedence[];
#define Ral_EXPRESSION_NESTING_PRECEDENCE_SHIFT 4

extern const char* const ral_serialized_operatorid_names[];



typedef enum
{
	Ral_SEPARATOR_COMMA,
	Ral_SEPARATOR_COLON,
	Ral_SEPARATOR_LPAREN,
	Ral_SEPARATOR_RPAREN,
	Ral_SEPARATOR_LBRACKET,
	Ral_SEPARATOR_RBRACKET,
	Ral_SEPARATOR_LBRACE,
	Ral_SEPARATOR_RBRACE,
	
	Ral_NUM_SEPARATORS,
	Ral_NOT_SEPARATOR = -1
} Ral_SeparatorID;

extern const char ral_separator_chars[];

Ral_SeparatorID Ral_CheckSeparator(const char character);

extern const char* const ral_serialized_separatorid_names[];





typedef enum
{
	Ral_TOKENTYPE_NULL,
	Ral_TOKENTYPE_KEYWORD,
	Ral_TOKENTYPE_IDENTIFIER,
	Ral_TOKENTYPE_INTLITERAL,
	Ral_TOKENTYPE_FLOATLITERAL,
	Ral_TOKENTYPE_STRINGLITERAL,
	Ral_TOKENTYPE_OPERATOR,
	Ral_TOKENTYPE_SEPARATOR,
	Ral_TOKENTYPE_ENDLINE
} Ral_TokenType;

typedef struct Ral_Token
{
	Ral_LISTLINKS(Ral_Token);

	char*			string;
	int				position;
	int				linenum;
	Ral_TokenType	type;
	Ral_KeywordID	keywordid;
	Ral_OperatorID	operatorid;
	Ral_SeparatorID	separatorid;
} Ral_Token;

void Ral_PrintToken(
	const Ral_Token* const token
);

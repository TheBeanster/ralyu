#pragma once

#include "ral_stdinclude.h"
#include "ral_list.h"



typedef enum
{
	Ral_KEYWORD_RETURN,

	Ral_KEYWORD_IF,
	Ral_KEYWORD_THEN,
	Ral_KEYWORD_ELSE,

	Ral_KEYWORD_FOR,
	Ral_KEYWORD_WHILE,
	Ral_KEYWORD_DO,
	Ral_KEYWORD_GOTO,

	Ral_KEYWORD_END,

	Ral_KEYWORD_PRINT,

	Ral_KEYWORD_VOID,
	Ral_KEYWORD_BOOL,
	Ral_KEYWORD_INT,
	Ral_KEYWORD_FLOAT,
	Ral_KEYWORD_CHAR,
	Ral_KEYWORD_STRING,

	Ral_KEYWORD_FALSE,
	Ral_KEYWORD_TRUE,

	Ral_KEYWORD_FUNCTION,

	Ral_NUM_KEYWORDS,
	Ral_NOT_KEYWORD = -1
} Ral_KeywordID;

extern const char* ral_keyword_strings[];

Ral_KeywordID Ral_CheckKeyword(const char* const str);

extern const char* ral_keywordid_names[];



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
	Ral_OPERATOR_MODULUS,				// %

	Ral_OPERATOR_MEMBERACCESS,			// .

	Ral_NUM_OPERATORS,
	Ral_NOT_OPERATOR = -1
} Ral_OperatorID;

extern const char* ral_operator_strings[];

Ral_OperatorID Ral_CheckOperator(const char* const str);

#define Ral_IS_ASSIGNMENT_OPERATOR(op) ((op >= 0 && op <= 4) ? Ral_TRUE : Ral_FALSE)
#define Ral_IS_UNARY_OPERATOR(op) ((op >= 5 && op <= 7) ? Ral_TRUE : Ral_FALSE)

extern const char* ral_operatorid_names[];

extern const int Ral_operator_precedence[];

#define Ral_EXPRESSION_NESTING_PRECEDENCE_SHIFT 4



typedef enum
{
	Ral_SEPARATOR_COMMA,
	Ral_SEPARATOR_LPAREN,
	Ral_SEPARATOR_RPAREN,
	Ral_SEPARATOR_COLON,

	Ral_NUM_SEPARATORS,
	Ral_NOT_SEPARATOR = -1
} Ral_SeparatorID;

extern const char ral_separator_chars[];

Ral_SeparatorID Ral_CheckSeparator(const char character);

extern const char* ral_separatorid_names[];





typedef enum
{
	Ral_TOKENTYPE_NULL,
	Ral_TOKENTYPE_KEYWORD,
	Ral_TOKENTYPE_IDENTIFIER,
	Ral_TOKENTYPE_NUMBERLITERAL,
	Ral_TOKENTYPE_STRINGLITERAL,
	Ral_TOKENTYPE_OPERATOR,
	Ral_TOKENTYPE_SEPARATOR
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
	double			numberliteral_value;
	char*			stringliteral_value;
} Ral_Token;

Ral_Token* Ral_CreateToken(
	const char* const	string,
	const int			start,
	const int			end,
	const int			linenum,
	const int			type
);

void Ral_DestroyToken(
	Ral_Token* const token
);

void Ral_PrintToken(
	const Ral_Token* const token
);

void Ral_PrintTokenValue(
	const Ral_Token* const token
);



typedef enum
{
	Ral_STATEMENTTYPE_NULL,

	Ral_STATEMENTTYPE_EXPRESSION,
	Ral_STATEMENTTYPE_DECLARATION,
	Ral_STATEMENTTYPE_FUNCTION,

	Ral_STATEMENTTYPE_IF,
	Ral_STATEMENTTYPE_ELSE,
	Ral_STATEMENTTYPE_FOR,
	Ral_STATEMENTTYPE_WHILE,
	Ral_STATEMENTTYPE_GOTO,

	Ral_STATEMENTTYPE_RETURN,
	Ral_STATEMENTTYPE_END,

	Ral_STATEMENTTYPE_PRINT,

	Ral_NUM_STATEMENTTYPES,

	Ral_STATREAD_PRINTEXPRESSION,

} Ral_StatementType;

extern const char* const ral_statementtype_names[];

typedef struct Ral_Statement
{
	Ral_LISTLINKS(Ral_Statement);

	Ral_StatementType type;

	struct Ral_SourceUnit* parentsource;

	Ral_Token*	tokens;
	int			numtokens;

	Ral_List	errormessages;

} Ral_Statement;

Ral_Statement* Ral_CreateStatement(
	const Ral_Token* const	begin,
	const Ral_Token* const	end,
	const Ral_StatementType type,
	const struct Ral_SourceUnit* const parentsource
);

void Ral_DestroyStatement(Ral_Statement* const statement);

void Ral_PrintStatement(const Ral_Statement* const statement);

void Ral_PrintStatementTokens(const Ral_Statement* const statement);



Ral_Bool Ral_TokenizeSourceUnit(
	struct Ral_SourceUnit* const source
);

Ral_Bool Ral_ValidateSourceUnit(
	const struct Ral_SourceUnit* const source
);

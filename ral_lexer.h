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
	Ral_CHARTYPE_NULL,			// Default type or any char not recognised
	Ral_CHARTYPE_ALPHA,			// Letter or underscore
	Ral_CHARTYPE_NUMBER,
	Ral_CHARTYPE_POINT,
	Ral_CHARTYPE_OPERATOR,
	Ral_CHARTYPE_SEPARATOR,
	Ral_CHARTYPE_SPACER,		// Space or tab
	Ral_CHARTYPE_ENDLINE,
	Ral_CHARTYPE_QUOTE,			// Double quotation marks for strings
	Ral_CHARTYPE_COMMENT,		// Comments start with # and end with an endline
} Ral_CharType;



typedef enum
{
	Ral_KEYWORD_IF,
	Ral_KEYWORD_THEN,
	Ral_KEYWORD_ELSE,
	Ral_KEYWORD_FOR,
	Ral_KEYWORD_WHILE,
	Ral_KEYWORD_DO,
	Ral_KEYWORD_FUNCTION,
	Ral_KEYWORD_RETURN,
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

	struct Ral_Object* expr_value;
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



typedef enum
{
	Ral_STATEMENTTYPE_NULL,
	Ral_STATEMENTTYPE_EXPRESSION,
	Ral_STATEMENTTYPE_FUNCTION,
	Ral_STATEMENTTYPE_RETURN,
	Ral_STATEMENTTYPE_IF,
	Ral_STATEMENTTYPE_ELSE,
	Ral_STATEMENTTYPE_FOR,
	Ral_STATEMENTTYPE_WHILE,
	Ral_STATEMENTTYPE_END,

	Ral_NUM_STATEMENTTYPES,
} Ral_StatementType;

extern const char* const ral_serialized_statementtype_names[];

typedef struct Ral_Statement
{
	Ral_LISTLINKS(Ral_Statement);

	Ral_StatementType	type;
	Ral_Token*			tokens;
	int					numtokens;

	struct Ral_SourceUnit*	parentsource; // The sourceunit that contains this statement
	int						index; // The index of this statement in the parentsources 'statements' array
} Ral_Statement;

Ral_Statement* Ral_CreateStatement(
	const Ral_Token* const	begin,
	const Ral_Token* const	end,
	const Ral_StatementType type
);

void Ral_DestroyStatement(Ral_Statement* const statement);

void Ral_PrintStatementString(const Ral_Statement* const statement);

void Ral_PrintStatementTokens(const Ral_Statement* const statement);



Ral_Bool Ral_ReadSourceStatements(
	Ral_List* const statements,
	const char* const string
);

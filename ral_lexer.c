#include "ral_lexer.h"

#include "ralu_string.h"
#include "ralu_list.h"
#include "ralu_memory.h"
#include <stdio.h>

#include "ral_logging.h"



typedef enum
{
	CHARTYPE_NULL,			// Default type or any char not recognised
	CHARTYPE_ALPHA,			// Letter or underscore
	CHARTYPE_NUMBER,
	CHARTYPE_POINT,
	CHARTYPE_OPERATOR,
	CHARTYPE_SEPARATOR,
	CHARTYPE_SPACER,		// Space or tab
	CHARTYPE_ENDLINE,
	CHARTYPE_DOUBLEQUOTES,	// Double quotation marks for strings
	CHARTYPE_COMMENT,		// Comments start with # and end with an endline
} chartype;

static chartype check_chartype(const char c)
{
	if (isalpha(c) || c == '_') return CHARTYPE_ALPHA;
	if (isdigit(c))				return CHARTYPE_NUMBER;
	if (c == '.')				return CHARTYPE_POINT;
	if (strchr("+-/*=<>!", c))	return CHARTYPE_OPERATOR;
	if (strchr(",:()[]{}", c))	return CHARTYPE_SEPARATOR;
	if (isblank(c))				return CHARTYPE_SPACER;
	if (c == '\n')				return CHARTYPE_ENDLINE;
	if (c == '\"')				return CHARTYPE_DOUBLEQUOTES;
	if (c == '#')				return CHARTYPE_COMMENT;
	return CHARTYPE_NULL;
}





// Only used in separate_tokens(), here to reduce duplicate code
#define PUSH_TOKEN																						\
	Ral_PushBackList(																					\
		tokens,																							\
		(Ral_ListNode*)Ral_CreateToken(source, curtoken_start, curtoken_end, curlinenum, curtoken_type)	\
	)

/// @brief Reads through a string character by character and divides it into separate tokens.
/// The types of these tokens must be determined later in the determine_token_types() function.
/// @param source Pointer to a string.
/// @param length The length of the source string.
/// @return A pointer to the list of tokens. Even though it shouldn't be able to return NULL, 
/// do check that just in case that changes later.
static Ral_List* separate_tokens(const char* const source, const int length)
{
	Ral_List* tokens = Ral_ALLOC_TYPE(Ral_List);

	int			curlinenum = 1;

	// Reading tokens works by checking where a token starts and where it ends, character by character,
	// by checking what the type of the current token is and how that token type would end.
	// For example, a text token ends when a char that isn't a letter, underscore or number apears, such as an operator char.
	// It will then push that token to the 'tokens' list, set the tokenchange flag and start reading the new token.

	// Where the current token being read starts in the source buffer.
	int			curtoken_start = 0;
	// Where the current token being read ends in the source buffer.
	int			curtoken_end = 0;

	// The type of the current token being read. 
	// This determines how it will check for token endings.
	chartype	curtoken_type = CHARTYPE_SPACER; // Starts as spacer since that will check for new tokens every char.

	Ral_Bool tokenchange = Ral_FALSE;

	// When reading a numberliteral, check if a decimal point has been found.
	// If this is true and you find another decimal point, that will be an error.
	Ral_Bool numberliteral_decimal = Ral_FALSE;



	for (int i = 0; i < length + 1; i++) // Plus 1 to include '\0'
	{
		// Looping over ever char in the sourcecode one by one

		char		cur_char = source[i];
		chartype	cur_chartype = check_chartype(cur_char);

		if (cur_chartype == CHARTYPE_ENDLINE) curlinenum++;

		tokenchange = Ral_FALSE;



		switch (curtoken_type)
		{
		case CHARTYPE_ALPHA:
			if (!(cur_chartype == CHARTYPE_ALPHA || cur_chartype == CHARTYPE_NUMBER))
			{
				// Text token ends when a character that isn't alpha or number appears
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_NUMBER:
			if (numberliteral_decimal)
			{
				if (cur_chartype == CHARTYPE_POINT)
				{
					// Two decimal points in one number error
					Ral_LOG_SYNTAXERROR(curlinenum, "Two decimal points in one number error!");
				}
			} else
			{
				if (cur_chartype == CHARTYPE_POINT)
					numberliteral_decimal = Ral_TRUE;
			}

			if (!(cur_chartype == CHARTYPE_NUMBER || cur_chartype == CHARTYPE_POINT))
			{
				if (cur_chartype == CHARTYPE_ALPHA)
				{
					// A letter or underscore directly after number
					Ral_LOG_SYNTAXERROR(curlinenum, "A letter or underscore directly after number!");
				}

				// Number token ends
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
				numberliteral_decimal = Ral_FALSE;
			}
			break;

		case CHARTYPE_POINT:
			if (curtoken_type == CHARTYPE_NUMBER)
			{
				curtoken_type = CHARTYPE_NUMBER;
			} else
			{
				curtoken_type = CHARTYPE_OPERATOR;
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_OPERATOR:
			if (cur_chartype == CHARTYPE_OPERATOR)
			{

			} else
			{
				// Operator token ends
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_SEPARATOR:
			// Separators are always 1 character so push it immedietly
			curtoken_end = i;
			PUSH_TOKEN;
			tokenchange = Ral_TRUE;
			break;



		case CHARTYPE_DOUBLEQUOTES:
			if (cur_chartype == CHARTYPE_ENDLINE)
			{
				Ral_LOG_SYNTAXERROR(curlinenum - 1, "String doesn't have closing quotes!");
				goto push_string_literal;
			}
			// String literals end when another double quote is found
			if (cur_chartype == CHARTYPE_DOUBLEQUOTES)
			{
			push_string_literal:
				curtoken_end = i + 1; // Plus 1 to include closing "
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_COMMENT:
			// Comments end with endlines
			if (cur_chartype == CHARTYPE_ENDLINE)
			{
				curtoken_end = i;
				// Don't push it to the list
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_ENDLINE:
			// Endlines are always 1 character so push it immedietly
			curtoken_end = i;
			PUSH_TOKEN;
			tokenchange = Ral_TRUE;
			break;



			// Default is error, it should be considered spacer
		default:
			// TODO Add warning about invalid characters
		case CHARTYPE_SPACER:
			if (cur_chartype == CHARTYPE_SPACER)
			{
				// Only blank characters so far
			} else
			{
				tokenchange = Ral_TRUE;
			}
			break;
		}



		if (tokenchange)
		{
			// After a token end, this will determine what the new token is.

			if ((curtoken_type == CHARTYPE_DOUBLEQUOTES) || (curtoken_type == CHARTYPE_COMMENT))
			{
				// Special cases for string and comment since they both stop at specific characters.
				// A double quotation mark for strings and endline for comments.
				// Set the curtoken_type as a spacer so the next type will be read properly.
				curtoken_type = CHARTYPE_SPACER;
			} else
			{
				// Set the new tokens start position as the current iterator position.
				curtoken_start = i;
				curtoken_type = cur_chartype;
			}
		}

	}



	// Check if the current type is a string literal
	if (curtoken_type == CHARTYPE_DOUBLEQUOTES)
	{
		Ral_LOG_SYNTAXERROR(curlinenum, "String doesn't have closing quotes!");
		curtoken_end = length;
		PUSH_TOKEN;
	}



	return tokens;

onerror:
	Ral_ClearList(tokens, &Ral_DestroyToken);
	Ral_FREE(tokens);
	return NULL;
}





/// @brief Goes through a list of tokens and determines more info about them. Such as what operator an operator token is,
/// or if a text token is a keyword or an identifier.
/// @param tokens The list of tokens to analyze.
/// @return TRUE if there were no errors or FALSE if some was found.
static Ral_Bool determine_token_types(const Ral_List* const tokens)
{
	Ral_Bool retvalue = Ral_TRUE;

	Ral_TokenType prev_token_type = Ral_TOKENTYPE_NULL;

	Ral_Token* iterator = (Ral_Token*)tokens->begin;
	while (iterator)
	{
		switch (iterator->type)
		{
		case CHARTYPE_ALPHA:
			iterator->keywordid = Ral_CheckKeyword(iterator->string);
			if (iterator->keywordid < 0) // NOT_KEYWORD is -1
				iterator->type = Ral_TOKENTYPE_IDENTIFIER;
			else
				iterator->type = Ral_TOKENTYPE_KEYWORD;
			break;

		case CHARTYPE_OPERATOR:
			iterator->operatorid = Ral_CheckOperator(iterator->string);
			if (iterator->operatorid < 0) // NOT_OPERATOR is -1
			{
				// Invalid operator
				Ral_LOG_SYNTAXERROR(iterator->linenum, "Invalid operator \"%s\"!", iterator->string);
				retvalue = Ral_FALSE;
			} else
			{
				// Check if unary negative or subtraction
				if (iterator->operatorid == Ral_OPERATOR_SUBTRACTION)
				{
					// If the previous token also was an operator then treat this minus sign as a unary negative.
					if (prev_token_type == Ral_TOKENTYPE_OPERATOR)
						iterator->operatorid = Ral_OPERATOR_NEGATIVE;
				}
			}
			iterator->type = Ral_TOKENTYPE_OPERATOR;
			break;

		case CHARTYPE_SEPARATOR:
			iterator->separatorid = Ral_CheckSeparator(iterator->string[0]);
			if (iterator->separatorid < 0) // NOT_SEPARATOR is -1
			{
				// Invalid separator
				Ral_LOG_SYNTAXERROR(iterator->linenum, "Invalid separator \"%s\"!", iterator->string);
				retvalue = Ral_FALSE;
			}
			iterator->type = Ral_TOKENTYPE_SEPARATOR;
			break;

		case CHARTYPE_NUMBER:
			if (strchr(iterator->string, '.')) // If number contins a decimal point then treat as float
				iterator->type = Ral_TOKENTYPE_FLOATLITERAL;
			else
				iterator->type = Ral_TOKENTYPE_INTLITERAL;
			break;

		case CHARTYPE_DOUBLEQUOTES:
			iterator->type = Ral_TOKENTYPE_STRINGLITERAL;
			break;

		case CHARTYPE_ENDLINE:
			iterator->type = Ral_TOKENTYPE_ENDLINE;
			break;

		default:
			
			retvalue = Ral_FALSE;
			break;
		}

		prev_token_type = iterator->type;
		iterator = (Ral_Token*)iterator->next;
	}



	return retvalue;
}





static Ral_List* separate_source_statements(
	const Ral_List* const tokens
)
{
	Ral_List* statements = Ral_ALLOC_TYPE(Ral_List);

	Ral_Token* cur_statementstart = tokens->begin;
	Ral_Token* iterator = tokens->begin;

	Ral_StatementType cur_statementtype = Ral_STATEMENTTYPE_NULL;
	Ral_StatementType expression_read_type = Ral_STATEMENTTYPE_NULL;

	int parenthesis_depth = 0;
	int bracket_depth = 0;
	int brace_depth = 0;

	for (int i = 0; i < tokens->itemcount; i++)
	{
		if		(iterator->separatorid == Ral_SEPARATOR_LBRACE) brace_depth++;
		else if (iterator->separatorid == Ral_SEPARATOR_RBRACE) brace_depth--;
		else if (iterator->separatorid == Ral_SEPARATOR_LPAREN) parenthesis_depth++;
		else if (iterator->separatorid == Ral_SEPARATOR_RPAREN) parenthesis_depth--;
		else if (iterator->separatorid == Ral_SEPARATOR_LBRACKET) bracket_depth++;
		else if (iterator->separatorid == Ral_SEPARATOR_RBRACKET) bracket_depth--;

		switch (cur_statementtype)
		{
		case Ral_STATEMENTTYPE_NULL:
			// Read new statement

			if (iterator->type == Ral_TOKENTYPE_ENDLINE) break; // Ignore empty lines

			switch (iterator->keywordid)
			{
			case Ral_KEYWORD_IF:
				cur_statementtype = Ral_STATEMENTTYPE_IF;
				break;

			case Ral_KEYWORD_ELSE:
				// Push the else statement immediatly
				Ral_PushBackList(
					statements,
					Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_ELSE)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
				break;

			case Ral_KEYWORD_FOR:
				cur_statementtype = Ral_STATEMENTTYPE_FOR;
				break;

			case Ral_KEYWORD_WHILE:
				cur_statementtype = Ral_STATEMENTTYPE_WHILE;
				break;

			case Ral_KEYWORD_RETURN:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION; // Continue reading as if it is an expression
				expression_read_type = Ral_STATEMENTTYPE_RETURN; // But remember that it is for a return statement
				break;

			case Ral_KEYWORD_BOOL:
			case Ral_KEYWORD_INT:
			case Ral_KEYWORD_FLOAT:
			case Ral_KEYWORD_STRING:
			case Ral_KEYWORD_TRUE:
			case Ral_KEYWORD_FALSE:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION;
				expression_read_type = Ral_STATEMENTTYPE_EXPRESSION;
				break;

			case Ral_KEYWORD_FUNCTION:
				cur_statementtype = Ral_STATEMENTTYPE_FUNCTION;
				break;

			case Ral_KEYWORD_STRUCT:
				cur_statementtype = Ral_STATEMENTTYPE_STRUCT;
				break;

			case Ral_KEYWORD_END:
				// Push the end statement immediatly
				Ral_PushBackList(
					statements,
					Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_END)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
				break;

			default:
				// First token in new statement is not a keyword

				// Treat as expression
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION;
				expression_read_type = Ral_STATEMENTTYPE_EXPRESSION;
				break;
			}

			cur_statementstart = iterator;
			break;



		case Ral_STATEMENTTYPE_IF:
			
			break;

		default:
			break;
		}
	}
}





Ral_Bool Ral_ParseSourceUnit(Ral_SourceUnit* const sourceunit, const char* const string, const int length)
{
	Ral_List* tokens = separate_tokens(string, length);
	if (!tokens)
	{
		printf("Could not parse source unit\n");
		return Ral_FALSE;
	}

	if (!determine_token_types(tokens))
	{
		Ral_ClearList(tokens, NULL);
		printf("Could not parse source unit\n");
		return Ral_FALSE;
	}

	Ral_Token* iter = (Ral_Token*)tokens->begin;
	while (iter)
	{
		Ral_PrintToken(iter);
		putchar('\n');
		iter = iter->next;
	}

	return Ral_TRUE;
}

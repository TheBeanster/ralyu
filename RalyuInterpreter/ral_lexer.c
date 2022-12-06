#include "ral_lexer.h"

#include <stdio.h>
#include <string.h>
#include "ralu_string.h"
#include "ralu_memory.h"
#include "ralu_list.h"

#include "ral_sourceunit.h"
#include "ral_logging.h"



// These shouldn't be bitsmasks, they likely just make the code slower
// TODO Change these to normal numbers
// The reason they are bitmasks is to make checking for multiple types faster,
// but the switch statement in separate_tokens() probably removes this benefit
typedef enum
{
	CHARTYPE_NULL			= 0,
	CHARTYPE_ALPHA			= 1, // Letter or underscore
	CHARTYPE_NUMBER			= 2,
	CHARTYPE_POINT			= 3,
	CHARTYPE_OPERATOR		= 4,
	CHARTYPE_SEPARATOR		= 5,
	CHARTYPE_SPACER			= 6, // Space or tab
	CHARTYPE_ENDLINE		= 7,
	CHARTYPE_DOUBLEQUOTES	= 8, // Double quotation marks for strings
	CHARTYPE_COMMENT		= 9, // Comments start with # and end with an endline
} chartype_t;



static chartype_t check_chartype(const char c)
{
	if (Ral_IsAlpha(c) || c == '_') return CHARTYPE_ALPHA;
	if (Ral_IsNumber(c)) return CHARTYPE_NUMBER;
	if (c == '.') return CHARTYPE_POINT;
	if (c == ' ' || c == '\t') return CHARTYPE_SPACER;
	if (c == '\n') return CHARTYPE_ENDLINE;
	if (Ral_IsCharIn(c, "+-=<>/*%!")) return CHARTYPE_OPERATOR;
	if (c == '\"') return CHARTYPE_DOUBLEQUOTES;
	if (Ral_IsCharIn(c, "(),:")) return CHARTYPE_SEPARATOR;
	if (c == '#') return CHARTYPE_COMMENT;
	return CHARTYPE_NULL;
}



// Only used in separate_tokens(), here to reduce duplicate code
#define PUSH_TOKEN																								\
	Ral_PushFrontList(																							\
		tokens,																									\
		(Ral_Listnode*)Ral_CreateToken(source->buffer, curtoken_start, curtoken_end, curlinenum, curtoken_type)	\
	)																											\


/// @brief Reads through a sourceunit character by character and divides it into separate tokens.
/// The types of these tokens must be determined later in the determine_token_types() function.
/// @param source Pointer to a sourceunit. Make sure the 'statements' array in it is empty.
/// @return A pointer to the list of tokens. Even though it shouldn't be able to return NULL, 
/// do check that just in case that might change later.
static Ral_List* separate_tokens(const Ral_SourceUnit* const source)
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
	chartype_t	curtoken_type = CHARTYPE_SPACER; // Starts as spacer since that will check for new tokens every char.
	
	Ral_Bool tokenchange = Ral_FALSE;

	// When reading a numberliteral, check if a decimal point has been found.
	// If this is true and you find another decimal point, that will be an error.
	Ral_Bool numberliteral_decimal = Ral_FALSE;



	for (int i = 0; i < source->length + 1; i++)
	{
		// Looping over ever char in the sourcecode one by one

		char		cur_char = source->buffer[i];
		chartype_t	cur_chartype = check_chartype(cur_char);

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
					Ral_PushError_SyntaxErrorPosition(
						source,
						i,
						-1,
						curlinenum,
						"Two decimal points in one number"
					);
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
					Ral_PushError_SyntaxErrorPosition(
						source,
						i,
						-1,
						curlinenum,
						"A letter or underscore directly after number"
					);
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
			// String literals end when another double quote is found
			if (cur_chartype == CHARTYPE_DOUBLEQUOTES)
			{
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



			// Default is error, it should be considered spacer
		default:
			// TODO Add warning about invalid characters
		case CHARTYPE_ENDLINE:
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
		// String doesn't have closing '"' error
		Ral_PushError_SyntaxErrorPosition(
			source,
			curtoken_start,
			-1,
			curlinenum, // TODO On multi line tokens this will be to the linenum of the end of the file
			"String doesn't have closing quotation marks"
		);
		curtoken_end = source->length;
		PUSH_TOKEN;
	}



	return tokens;
}







/// @brief Goes through a list of tokens and determines more info about them. Such as what operator an operator token is,
/// or if a text token is a keyword or an identifier.
/// @param tokens The list of tokens to analyze.
/// @param source Source is here for error messages.
/// @return TRUE if there were no errors or FALSE if some was found.
static Ral_Bool determine_token_types(const Ral_List* const tokens, const Ral_SourceUnit* const source)
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
				Ral_PushError_SyntaxErrorPosition(
					source,
					iterator->position,
					strlen(iterator->string),
					iterator->linenum,
					"Invalid operator"
				);
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
				Ral_PushError_SyntaxErrorPosition(
					source,
					iterator->position,
					strlen(iterator->string),
					iterator->linenum,
					"Invalid separator"
				);
				retvalue = Ral_FALSE;
			}
			iterator->type = Ral_TOKENTYPE_SEPARATOR;
			break;

		case CHARTYPE_NUMBER:
			iterator->numberliteral_value = atof(iterator->string);
			iterator->type = Ral_TOKENTYPE_NUMBERLITERAL;
			break;

		case CHARTYPE_DOUBLEQUOTES:
			iterator->stringliteral_value = Ral_CreateCutString(iterator->string, 1, strlen(iterator->string) - 2);
			iterator->type = Ral_TOKENTYPE_STRINGLITERAL;
			break;

		default:
			// If this happens, something really bad must've happened
			Ral_PushError_SyntaxErrorPosition(
				source,
				iterator->position,
				strlen(iterator->string),
				iterator->linenum,
				"A token was read as an invalid type! This is a critical error in the lexer! At line " Ral_STRINGIFY(__LINE__) " in ral_lexer.c"
			);
			return Ral_FALSE;
			break;
		}

		prev_token_type = iterator->type;
		iterator = (Ral_Token*)iterator->next;
	}



	return retvalue;
}







// TODO Add comments

static Ral_List* separate_source_statements(
	const Ral_List* const tokens,
	const Ral_SourceUnit* const source
)
{
	Ral_List* statements = Ral_ALLOC_TYPE(Ral_List);

	Ral_Token* statement_starttoken = tokens->begin;
	Ral_Token* iterator = tokens->begin;

	Ral_StatementType cur_statementtype = Ral_STATEMENTTYPE_NULL;

	// Used to keep track of what the expression being read is for
	Ral_StatementType expression_statement_type = Ral_STATEMENTTYPE_NULL;

	for (int i = 0; i < tokens->itemcount; i++)
	{
		switch (cur_statementtype)
		{
		case Ral_STATEMENTTYPE_NULL:
			// Start of new statement

			switch (iterator->keywordid)
			{
			case Ral_KEYWORD_IF:
				cur_statementtype = Ral_STATEMENTTYPE_IF;
				break;

				/*case Ral_KEYWORD_BOOL:
				case Ral_KEYWORD_INT:
				case Ral_KEYWORD_FLOAT:
				case Ral_KEYWORD_CHAR:
				case Ral_KEYWORD_STRING:
				cur_statementtype = Ral_STATEMENTTYPE_DECLARATION;
				break;*/

			case Ral_KEYWORD_FOR:
				cur_statementtype = Ral_STATEMENTTYPE_FOR;
				break;

			case Ral_KEYWORD_WHILE:
				cur_statementtype = Ral_STATEMENTTYPE_WHILE;
				break;

			case Ral_KEYWORD_RETURN:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION; // Continue reading as if it is an expression
				expression_statement_type = Ral_STATEMENTTYPE_RETURN; // But remember that it is for a return statement
				break;

			case Ral_KEYWORD_PRINT:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION; // Continue reading as if it is an expression
				expression_statement_type = Ral_STATEMENTTYPE_PRINT;// But remember that it is for a print statement
				break;

			case Ral_KEYWORD_ELSE:
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_ELSE, source)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
				break;

			case Ral_KEYWORD_END:
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_END, source)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
				break;

			case Ral_KEYWORD_BOOL:
			case Ral_KEYWORD_INT:
			case Ral_KEYWORD_FLOAT:
			case Ral_KEYWORD_CHAR:
			case Ral_KEYWORD_STRING:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION;
				expression_statement_type = Ral_STATEMENTTYPE_DECLARATION;
				break;

			case Ral_KEYWORD_FUNCTION:
				cur_statementtype = Ral_STATEMENTTYPE_FUNCTION;
				break;

				// Keywords that are invalid as the start of statements
			case Ral_KEYWORD_DO:
				break;

			case Ral_KEYWORD_FALSE:
				break;

			case Ral_KEYWORD_TRUE:
				break;

			default:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION;
				expression_statement_type = Ral_STATEMENTTYPE_EXPRESSION;

				// Check if the expression is a single token
				if (!iterator->next ||
					iterator->next->type == Ral_TOKENTYPE_IDENTIFIER ||
					iterator->next->type == Ral_TOKENTYPE_NUMBERLITERAL ||
					iterator->next->type == Ral_TOKENTYPE_STRINGLITERAL ||
					iterator->next->type == Ral_SEPARATOR_LPAREN)
				{
					Ral_PushFrontList(
						statements,
						Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_EXPRESSION, source)
					);
					cur_statementtype = Ral_STATEMENTTYPE_NULL;
				}
				else if (iterator->next->type == Ral_TOKENTYPE_KEYWORD)
				{
					if (!(
						iterator->next->keywordid == Ral_KEYWORD_FALSE ||
						iterator->next->keywordid == Ral_KEYWORD_TRUE))
					{
						Ral_PushFrontList(
							statements,
							Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_EXPRESSION, source)
						);
						cur_statementtype = Ral_STATEMENTTYPE_NULL;
					}
				}

				break;
			}

			statement_starttoken = iterator;
			break;



		case Ral_STATEMENTTYPE_IF:
			if (iterator->keywordid == Ral_KEYWORD_THEN)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_IF, source)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_FOR:
			if (iterator->keywordid == Ral_KEYWORD_DO)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_FOR, source)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_WHILE:
			if (iterator->keywordid == Ral_KEYWORD_DO)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_WHILE, source)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_GOTO:
			// If reading a goto statement then it will finish by the next token, since the next token should be an identifier
			Ral_PushFrontList(
				statements,
				Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_GOTO, source)
			);
			cur_statementtype = Ral_STATEMENTTYPE_NULL;
			break;

		case Ral_STATEMENTTYPE_FUNCTION:
			if (iterator->separatorid == Ral_SEPARATOR_RPAREN)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_FUNCTION, source)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;



		case Ral_STATEMENTTYPE_EXPRESSION:
			if (iterator->type == Ral_TOKENTYPE_IDENTIFIER ||
				iterator->type == Ral_TOKENTYPE_NUMBERLITERAL ||
				iterator->type == Ral_TOKENTYPE_STRINGLITERAL ||
				iterator->separatorid == Ral_SEPARATOR_RPAREN)
			{
				if (!iterator->next ||
					iterator->next->type == Ral_TOKENTYPE_IDENTIFIER ||
					iterator->next->type == Ral_TOKENTYPE_NUMBERLITERAL ||
					iterator->next->type == Ral_TOKENTYPE_STRINGLITERAL ||
					iterator->next->type == Ral_SEPARATOR_LPAREN)
				{
					Ral_PushFrontList(
						statements,
						Ral_CreateStatement(statement_starttoken, iterator, expression_statement_type, source)
					);
					cur_statementtype = Ral_STATEMENTTYPE_NULL;
				}

				else if (iterator->next->type == Ral_TOKENTYPE_KEYWORD)
				{
					if (!(
						iterator->next->keywordid == Ral_KEYWORD_FALSE ||
						iterator->next->keywordid == Ral_KEYWORD_TRUE))
					{
						Ral_PushFrontList(
							statements,
							Ral_CreateStatement(statement_starttoken, iterator, expression_statement_type, source)
						);
						cur_statementtype = Ral_STATEMENTTYPE_NULL;
					}
				}

			}
			break;

		default:
			break;
		}

		iterator = iterator->next;
	}

	return statements;



onerror:
	Ral_ClearList(statements, &Ral_DestroyStatement);
	Ral_FREE(statements);
	return NULL;
}







Ral_Bool Ral_TokenizeSourceUnit(Ral_SourceUnit* const source)
{
	Ral_List* tokens = separate_tokens(source);

	if (!tokens)
	{
		printf("OH SHIT THIS BAD\n");
		return Ral_FALSE;
	}

	if (!determine_token_types(tokens, source))
	{
		printf("Could not determine token types\n");
		Ral_ClearList(tokens, &Ral_DestroyToken);
		Ral_FREE(tokens);
		return Ral_FALSE;
	}

	Ral_List* statements = separate_source_statements(tokens, source);

	if (!statements)
	{
		printf("Could not separate into statements\n");
		Ral_ClearList(tokens, &Ral_DestroyToken);
		Ral_FREE(tokens);
		return Ral_FALSE;
	}

	int numstatements = statements->itemcount;
	source->numstatements = numstatements;
	source->statements = Ral_CALLOC(numstatements, sizeof(Ral_Statement));
	Ral_Statement* iterator = statements->begin;
	for (int i = 0; i < numstatements; i++)
	{
		source->statements[i] = *iterator;
		source->statements[i].index = i;
		iterator = iterator->next;
	}



	// Destroy the lists now that they've been copied to arrays
	iterator = tokens->begin;
	while (iterator)
	{
		Ral_Token* del = iterator;
		iterator = iterator->next;
		Ral_FREE(del);
	}
	Ral_FREE(tokens);
	iterator = statements->begin;
	while (iterator)
	{
		Ral_Statement* del = iterator;
		iterator = iterator->next;
		Ral_FREE(del);
	}
	Ral_FREE(statements);



	if (source->errormessages.itemcount > 0)
	{
		// If any errors were found, don't run
		return Ral_FALSE;
	}



	return Ral_TRUE;
}

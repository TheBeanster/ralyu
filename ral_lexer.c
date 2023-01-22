#include "ral_lexer.h"

#include "ralu_string.h"
#include "ralu_list.h"
#include "ralu_memory.h"
#include <stdio.h>

#include "ral_logging.h"
#include "ral_object.h"




static Ral_CharType check_chartype(const char c)
{
	if (isalpha(c) || c == '_') return Ral_CHARTYPE_ALPHA;
	if (isdigit(c))				return Ral_CHARTYPE_NUMBER;
	if (c == '.')				return Ral_CHARTYPE_POINT;
	if (strchr("+-/*=<>!", c))	return Ral_CHARTYPE_OPERATOR;
	if (strchr(",:()[]{}", c))	return Ral_CHARTYPE_SEPARATOR;
	if (isblank(c))				return Ral_CHARTYPE_SPACER;
	if (c == '\n' || c == ';')	return Ral_CHARTYPE_ENDLINE; // Semicolon acts the same as endline
	if (c == '\"' || c == '\'')	return Ral_CHARTYPE_QUOTE;
	if (c == '#')				return Ral_CHARTYPE_COMMENT;
	return Ral_CHARTYPE_NULL;
}





// Only used in separate_tokens(), here to reduce duplicate code
#define PUSH_TOKEN																						\
	Ral_PushBackList(																					\
		tokens,																							\
		(Ral_ListNode*)Ral_CreateToken(source, curtoken_start, curtoken_end, curlinenum, curtoken_type)	\
	)

/// @brief Reads through a string character by character and divides it into separate tokens.
/// The types of these tokens must be determined later in the determine_token_types() function.
/// @param tokens Pointer to an empty list to put the tokens into.
/// @param source Pointer to a string.
/// @return The number of errors found.
static int separate_tokens(Ral_List* const tokens, const char* const source)
{
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
	Ral_CharType	curtoken_type = Ral_CHARTYPE_SPACER; // Starts as spacer since that will check for new tokens every char.

	Ral_Bool tokenchange = Ral_FALSE;

	// When reading a numberliteral, check if a decimal point has been found.
	// If this is true and you find another decimal point, that will be an error.
	Ral_Bool numberliteral_decimal = Ral_FALSE;

	int			numerrors = 0;



	int i = 0;
	while(1)
	{
		// Looping over every char in the sourcecode one by one

		char		cur_char = source[i];
		Ral_CharType	cur_chartype = check_chartype(cur_char);

		if (cur_chartype == Ral_CHARTYPE_ENDLINE) curlinenum++;

		tokenchange = Ral_FALSE;



		switch (curtoken_type)
		{
		case Ral_CHARTYPE_ALPHA:
			if (!(cur_chartype == Ral_CHARTYPE_ALPHA || cur_chartype == Ral_CHARTYPE_NUMBER))
			{
				// Text token ends when a character that isn't alpha or number appears
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case Ral_CHARTYPE_NUMBER:
			if (numberliteral_decimal)
			{
				if (cur_chartype == Ral_CHARTYPE_POINT)
				{
					// Two decimal points in one number error
					Ral_LOG_SYNTAXERROR(curlinenum, "Two decimal points in one number!");
					numerrors++;
				}
			} else
			{
				if (cur_chartype == Ral_CHARTYPE_POINT)
					numberliteral_decimal = Ral_TRUE;
			}

			if (!(cur_chartype == Ral_CHARTYPE_NUMBER || cur_chartype == Ral_CHARTYPE_POINT))
			{
				if (cur_chartype == Ral_CHARTYPE_ALPHA)
				{
					// A letter or underscore directly after number
					Ral_LOG_SYNTAXERROR(curlinenum, "A letter or underscore directly after number!");
					numerrors++;
				}

				// Number token ends
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
				numberliteral_decimal = Ral_FALSE;
			}
			break;

		case Ral_CHARTYPE_POINT:
			if (curtoken_type == Ral_CHARTYPE_NUMBER)
			{
				curtoken_type = Ral_CHARTYPE_NUMBER;
			} else
			{
				curtoken_type = Ral_CHARTYPE_OPERATOR;
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case Ral_CHARTYPE_OPERATOR:
			if (cur_chartype == Ral_CHARTYPE_OPERATOR)
			{

			} else
			{
				// Operator token ends
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case Ral_CHARTYPE_SEPARATOR:
			// Separators are always 1 character so push it immedietly
			curtoken_end = i;
			PUSH_TOKEN;
			tokenchange = Ral_TRUE;
			break;



		case Ral_CHARTYPE_QUOTE:
			if (cur_chartype == Ral_CHARTYPE_ENDLINE)
			{
				Ral_LOG_SYNTAXERROR(curlinenum - 1, "String doesn't have closing quotes!");
				numerrors++;
				goto push_string_literal;
			}
			// String literals end when another double quote is found
			if (cur_chartype == Ral_CHARTYPE_QUOTE)
			{
			push_string_literal:
				curtoken_end = i + 1; // Plus 1 to include closing "
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case Ral_CHARTYPE_COMMENT:
			// Comments end with endlines
			if (cur_chartype == Ral_CHARTYPE_ENDLINE)
			{
				curtoken_end = i;
				// Don't push it to the list
				tokenchange = Ral_TRUE;
			}
			break;



		case Ral_CHARTYPE_ENDLINE:
			// Endlines are always 1 character so push it immedietly
			curtoken_end = i;
			PUSH_TOKEN;
			tokenchange = Ral_TRUE;
			break;



			// Default is error, it should be considered spacer
		default:
			// TODO Add warning about invalid characters
		case Ral_CHARTYPE_SPACER:
			if (cur_chartype == Ral_CHARTYPE_SPACER)
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

			if ((curtoken_type == Ral_CHARTYPE_QUOTE) || (curtoken_type == Ral_CHARTYPE_COMMENT))
			{
				// Special cases for string and comment since they both stop at specific characters.
				// A double quotation mark for strings and endline for comments.
				// Set the curtoken_type as a spacer so the next type will be read properly.
				curtoken_type = Ral_CHARTYPE_SPACER;
			} else
			{
				// Set the new tokens start position as the current iterator position.
				curtoken_start = i;
				curtoken_type = cur_chartype;
			}
		}

		if (cur_char == '\0') break;
		i++; // Next char
	}



	// Check if the current type is a string literal
	if (curtoken_type == Ral_CHARTYPE_QUOTE)
	{
		Ral_LOG_SYNTAXERROR(curlinenum, "String doesn't have closing quotes!");
		numerrors++;
		curtoken_end = i;
		PUSH_TOKEN;
	}



	// Add an endline token at the end to make sure parsing works
	Ral_PushBackList(
		tokens,
		(Ral_ListNode*)Ral_CreateToken(NULL, 0, 0, curlinenum, Ral_CHARTYPE_ENDLINE)
	);



	return numerrors;
}





/// @brief Goes through a list of tokens and determines more info about them. Such as what operator an operator token is,
/// or if a text token is a keyword or an identifier.
/// @param tokens The list of tokens to analyze.
/// @return The number of errors found.
static int determine_token_types(Ral_List* const tokens)
{
	Ral_TokenType prev_token_type = Ral_TOKENTYPE_NULL;

	int numerrors = 0;

	Ral_Token* iterator = (Ral_Token*)tokens->begin;
	while (iterator)
	{
		switch (iterator->type)
		{
		case Ral_CHARTYPE_ALPHA:
			iterator->keywordid = Ral_CheckKeyword(iterator->string);
			if (iterator->keywordid < 0) // NOT_KEYWORD is -1
				iterator->type = Ral_TOKENTYPE_IDENTIFIER;
			else
				iterator->type = Ral_TOKENTYPE_KEYWORD;
			break;

		case Ral_CHARTYPE_OPERATOR:
			iterator->operatorid = Ral_CheckOperator(iterator->string);
			if (iterator->operatorid < 0) // NOT_OPERATOR is -1
			{
				// Invalid operator
				Ral_LOG_SYNTAXERROR(iterator->linenum, "Invalid operator \"%s\"!", iterator->string);
				numerrors++;
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

		case Ral_CHARTYPE_SEPARATOR:
			iterator->separatorid = Ral_CheckSeparator(iterator->string[0]);
			if (iterator->separatorid < 0) // NOT_SEPARATOR is -1
			{
				// Invalid separator
				Ral_LOG_SYNTAXERROR(iterator->linenum, "Invalid separator \"%s\"!", iterator->string);
				numerrors++;
			}
			iterator->type = Ral_TOKENTYPE_SEPARATOR;
			break;

		case Ral_CHARTYPE_NUMBER:
			if (strchr(iterator->string, '.')) // If number contins a decimal point then treat as float
				iterator->type = Ral_TOKENTYPE_FLOATLITERAL;
			else
				iterator->type = Ral_TOKENTYPE_INTLITERAL;
			break;

		case Ral_CHARTYPE_QUOTE:
			iterator->type = Ral_TOKENTYPE_STRINGLITERAL;
			break;

		case Ral_CHARTYPE_ENDLINE:
			iterator->type = Ral_TOKENTYPE_ENDLINE;
			break;

		default:
			Ral_LOG_SYNTAXERROR(iterator->linenum, "Invalid token \"%s\"!", iterator->string);
			numerrors++;
			Ral_Token* deltoken = iterator;
			iterator = iterator->next;
			Ral_UnlinkFromList(tokens, deltoken);
			Ral_DestroyToken(deltoken);
			continue;
		}

		prev_token_type = iterator->type;
		iterator = iterator->next;
	}

	return numerrors;
}





static int separate_source_statements(
	Ral_List* const statements,
	const Ral_List* const tokens
)
{
	Ral_Token* cur_statementstart = tokens->begin;
	Ral_Token* iterator = tokens->begin;

	Ral_StatementType cur_statementtype = Ral_STATEMENTTYPE_NULL;
	Ral_StatementType expression_read_type = Ral_STATEMENTTYPE_NULL;

	int parenthesis_depth = 0;
	int bracket_depth = 0;
	int brace_depth = 0;

	int start_parenthesis_depth = 0;
	int start_bracket_depth = 0;
	int start_brace_depth = 0;

	int numerrors = 0;

	for (int i = 0; i < tokens->itemcount; i++)
	{
		switch (iterator->separatorid)
		{
		case Ral_SEPARATOR_LPAREN:		parenthesis_depth++;	break;
		case Ral_SEPARATOR_RPAREN:		parenthesis_depth--;	break;
		case Ral_SEPARATOR_LBRACKET:	bracket_depth++;		break;
		case Ral_SEPARATOR_RBRACKET:	bracket_depth--;		break;
		case Ral_SEPARATOR_LBRACE:		brace_depth++;			break;
		case Ral_SEPARATOR_RBRACE:		brace_depth--;			break;
		}
		
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

			case Ral_KEYWORD_TRUE:
			case Ral_KEYWORD_FALSE:
				cur_statementtype = Ral_STATEMENTTYPE_EXPRESSION;
				expression_read_type = Ral_STATEMENTTYPE_EXPRESSION;
				break;

			case Ral_KEYWORD_FUNCTION:
				cur_statementtype = Ral_STATEMENTTYPE_FUNCTION;
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

				// Invalid keywords at statement start
			case Ral_KEYWORD_DO:
			case Ral_KEYWORD_THEN:
				Ral_LOG_SYNTAXERROR(iterator->linenum, "Invalid statement start keyword \"%s\"!", iterator->string);
				numerrors++;
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
				break;
			}

			start_parenthesis_depth = parenthesis_depth;
			start_bracket_depth = bracket_depth;
			start_brace_depth = brace_depth;

			cur_statementstart = iterator;
			break;



		case Ral_STATEMENTTYPE_IF:
			if (iterator->keywordid == Ral_KEYWORD_THEN)
			{
				Ral_PushBackList(
					statements,
					Ral_CreateStatement(cur_statementstart, iterator, Ral_STATEMENTTYPE_IF)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_FOR:
			if (iterator->keywordid == Ral_KEYWORD_DO)
			{
				Ral_PushBackList(
					statements,
					Ral_CreateStatement(cur_statementstart, iterator, Ral_STATEMENTTYPE_FOR)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_WHILE:
			if (iterator->keywordid == Ral_KEYWORD_DO)
			{
				Ral_PushBackList(
					statements,
					Ral_CreateStatement(cur_statementstart, iterator, Ral_STATEMENTTYPE_WHILE)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;



		case Ral_STATEMENTTYPE_FUNCTION:
			if (iterator->type == Ral_TOKENTYPE_ENDLINE)
			{
				if ((start_parenthesis_depth	>= parenthesis_depth) &&
					(start_bracket_depth		>= bracket_depth) &&
					(start_brace_depth			>= brace_depth))
				{
					Ral_PushBackList(
						statements,
						Ral_CreateStatement(cur_statementstart, iterator, Ral_STATEMENTTYPE_FUNCTION)
					);
					cur_statementtype = Ral_STATEMENTTYPE_NULL;
				}
			}
			break;



		case Ral_STATEMENTTYPE_EXPRESSION:
			if (iterator->type == Ral_TOKENTYPE_ENDLINE)
			{
				if ((start_parenthesis_depth	>= parenthesis_depth) &&
					(start_bracket_depth		>= bracket_depth) &&
					(start_brace_depth			>= brace_depth))
				{
					// TODO It is probably possible to implement an error for statements that have too many parentheses or similar

					Ral_PushBackList(
						statements,
						Ral_CreateStatement(cur_statementstart, iterator, expression_read_type)
					);
					cur_statementtype = Ral_STATEMENTTYPE_NULL;
				}
			}
			break;

		default:
			break;
		}

		iterator = iterator->next;
	}

	return numerrors;
}





Ral_Bool parse_source_statements(
	Ral_List* const statements,
	const char* const string
)
{
	Ral_List tokens = { 0 };

	int numerrors = 0;

	numerrors += separate_tokens(&tokens, string);
	numerrors += determine_token_types(&tokens);
	
	// Print the tokens
	/*Ral_Token* token_iter = (Ral_Token*)tokens.begin;
	while (token_iter)
	{
		Ral_PrintToken(token_iter);
		putchar('\n');
		token_iter = token_iter->next;
	}*/

	numerrors += separate_source_statements(statements, &tokens);

	// Clear the tokens list
	Ral_ClearList(&tokens, NULL); // Instead of calling Ral_DestroyToken() call free. 
	// This won't free the string of the token which is now owned by the statement array tokens
	
	if (numerrors > 0)
	{
		printf("Could not parse source unit\nErrors: %i\n", numerrors);
		Ral_ClearList(statements, &Ral_DestroyStatement);
		return Ral_FALSE;
	}

	/*for (int i = 0; i < statements->itemcount; i++)
	{
		Ral_Statement* statement = (Ral_Statement*)statements->begin;
		for (int j = 0; j < statement->numtokens; j++)
		{
			statement->tokens[j].prev = NULL;
			statement->tokens[j].next = NULL;
		}
	}*/

	// Print statements
	/*Ral_Statement* statement_iter = (Ral_Statement*)statements->begin;
	while (statement_iter)
	{
		Ral_PrintStatementTokens(statement_iter);
		putchar('\n');
		statement_iter = statement_iter->next;
	}*/

	return Ral_TRUE;
}





Ral_Bool Ral_ReadSourceStatements(
	Ral_List* const statements,
	const char* const string
)
{
	if (!statements) return Ral_FALSE;
	if (statements->itemcount > 0) return Ral_FALSE;
	if (!string) return Ral_FALSE;
	if (string[0] == '\0') return Ral_FALSE;

	if (!parse_source_statements(statements, string))
	{
		return Ral_FALSE;
	}

	return Ral_TRUE;
}

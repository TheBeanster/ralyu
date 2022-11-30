#include "ral_lexer.h"

#include <stdio.h>
#include <string.h>
#include "ral_string.h"
#include "ral_memory.h"
#include "ral_list.h"

#include "ral_sourceunit.h"



typedef enum
{
	CHARTYPE_NULL			= (1U << 0),
	CHARTYPE_ALPHA			= (1U << 1), // Letter or underscore
	CHARTYPE_NUMBER			= (1U << 2),
	CHARTYPE_POINT			= (1U << 3),
	CHARTYPE_OPERATOR		= (1U << 4),
	CHARTYPE_SEPARATOR		= (1U << 5),
	CHARTYPE_SPACER			= (1U << 6), // Space or tab
	CHARTYPE_ENDLINE		= (1U << 7),
	CHARTYPE_DOUBLEQUOTES	= (1U << 8), // Double quotation marks for strings
	CHARTYPE_COMMENT		= (1U << 9), // Comments start with # and end with an endline
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




#define PUSH_TOKEN																								\
	Ral_PushFrontList(																							\
		tokens,																									\
		(Ral_Listnode*)Ral_CreateToken(source->buffer, curtoken_start, curtoken_end, curlinenum, curtoken_type)	\
	)																											\

static Ral_List* separate_tokens(const Ral_SourceUnit* const source)
{
	Ral_List* tokens = Ral_ALLOC_TYPE(Ral_List);

	int			curlinenum = 1;

	int			curtoken_start = 0;
	int			curtoken_end = 0;
	chartype_t	curtoken_type = CHARTYPE_SPACER;

	Ral_Bool tokenchange = Ral_FALSE;

	Ral_Bool numberliteral_decimal = Ral_FALSE;



	for (int i = 0; i < (int)source->length + 1; i++)
	{
		char		cur_char = source->buffer[i];
		chartype_t	cur_type = check_chartype(cur_char);

		if (cur_type & CHARTYPE_ENDLINE) curlinenum++;

		tokenchange = Ral_FALSE;



		switch (curtoken_type)
		{
		case CHARTYPE_ALPHA:
			if (!(cur_type & (CHARTYPE_ALPHA | CHARTYPE_NUMBER)))
			{
				// Text token ends
				curtoken_end = i;
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_NUMBER:
			if (numberliteral_decimal)
			{
				if (cur_type & CHARTYPE_POINT)
				{
					// Two decimal points in one number
					goto onerror;
				}
			} else
			{
				if (cur_type & CHARTYPE_POINT)
					numberliteral_decimal = Ral_TRUE;
			}

			if (!(cur_type & (CHARTYPE_NUMBER | CHARTYPE_POINT)))
			{
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
			if (cur_type & CHARTYPE_OPERATOR)
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
			if (cur_type & CHARTYPE_DOUBLEQUOTES)
			{
				curtoken_end = i + 1; // Plus 1 to include closing "
				PUSH_TOKEN;
				tokenchange = Ral_TRUE;
			}
			break;



		case CHARTYPE_COMMENT:
			// Comments end with endlines
			if (cur_type & CHARTYPE_ENDLINE)
			{
				curtoken_end = i + 1;
				tokenchange = Ral_TRUE;
			}
			break;



			// Default is error, it can be considered spacer
		default:
		case CHARTYPE_SPACER:
			if (cur_type & CHARTYPE_SPACER)
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
			if ((curtoken_type & CHARTYPE_DOUBLEQUOTES) || (curtoken_type & CHARTYPE_COMMENT))
			{
				// If ending a string or comment
				curtoken_type = CHARTYPE_SPACER;
			} else
			{
				curtoken_start = i;

				curtoken_type = cur_type;
			}
		}

	}



	// String literal has no closing double quotes
	if (curtoken_type & CHARTYPE_DOUBLEQUOTES)
	{
		// String doesn't have closing '"'
		goto onerror;
	}



	return tokens;



onerror:
	Ral_ClearList(
		tokens,
		&Ral_DestroyToken
	);
	return NULL;
}





static Ral_Bool determine_token_types(const Ral_List* const tokens)
{
	Ral_TokenType last_token_type = Ral_TOKENTYPE_NULL;

	Ral_Token* iterator = (Ral_Token*)tokens->begin;
	while (iterator)
	{
		switch (iterator->type)
		{
		case CHARTYPE_ALPHA:
			iterator->keywordid = Ral_CheckKeyword(iterator->string);
			if (iterator->keywordid == Ral_NOT_KEYWORD)
				iterator->type = Ral_TOKENTYPE_IDENTIFIER;
			else
				iterator->type = Ral_TOKENTYPE_KEYWORD;
			break;

		case CHARTYPE_OPERATOR:
			iterator->operatorid = Ral_CheckOperator(iterator->string);
			if (iterator->operatorid < 0)
			{
				// Invalid operator
				goto onerror;
			} else
			{
				// Check if unary negative or subtraction
				if (iterator->operatorid == Ral_OPERATOR_SUBTRACTION)
				{
					if (last_token_type == Ral_TOKENTYPE_OPERATOR)
						iterator->operatorid = Ral_OPERATOR_NEGATIVE;
				}

				iterator->type = Ral_TOKENTYPE_OPERATOR;
			}
			break;

		case CHARTYPE_SEPARATOR:
			iterator->separatorid = Ral_CheckSeparator(iterator->string[0]);
			if (iterator->separatorid < 0)
			{
				// Invalid separator
				goto onerror;
			} else
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
			break;
		}

		last_token_type = iterator->type;
		iterator = (Ral_Token*)iterator->next;
	}



	return Ral_TRUE;

onerror:

	return Ral_FALSE;
}







static Ral_List* separate_source_statements(const Ral_List* const tokens)
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
					Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_ELSE)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
				break;

			case Ral_KEYWORD_END:
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_END)
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
						Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_EXPRESSION)
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
							Ral_CreateStatement(iterator, iterator, Ral_STATEMENTTYPE_EXPRESSION)
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
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_IF)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_FOR:
			if (iterator->keywordid == Ral_KEYWORD_DO)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_FOR)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_WHILE:
			if (iterator->keywordid == Ral_KEYWORD_DO)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_WHILE)
				);
				cur_statementtype = Ral_STATEMENTTYPE_NULL;
			}
			break;

		case Ral_STATEMENTTYPE_GOTO:
			// If reading a goto statement then it will finish by the next token, since the next token should be an identifier
			Ral_PushFrontList(
				statements,
				Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_GOTO)
			);
			cur_statementtype = Ral_STATEMENTTYPE_NULL;
			break;

		case Ral_STATEMENTTYPE_FUNCTION:
			if (iterator->separatorid == Ral_SEPARATOR_RPAREN)
			{
				Ral_PushFrontList(
					statements,
					Ral_CreateStatement(statement_starttoken, iterator, Ral_STATEMENTTYPE_FUNCTION)
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
						Ral_CreateStatement(statement_starttoken, iterator, expression_statement_type)
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
							Ral_CreateStatement(statement_starttoken, iterator, expression_statement_type)
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

	if (!determine_token_types(tokens))
	{
		printf("Could not determine token types\n");
		Ral_ClearList(tokens, &Ral_DestroyToken);
		Ral_FREE(tokens);
		return Ral_FALSE;
	}

	Ral_List* statements = separate_source_statements(tokens);

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
		printf("%s\n\t", ral_statementtype_names[iterator->type]);
		for (int i = 0; i < iterator->numtokens; i++)
		{
			Ral_PrintTokenValue(&iterator->tokens[i]);
			printf("\n\t");
		}

		putchar('\n');

		source->statements[i] = *iterator;
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

	return Ral_TRUE;
}

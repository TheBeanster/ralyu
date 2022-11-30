#include "ral_logging.h"

#include <stdio.h>
#include "ral_memory.h"



int Ral_PrintLineOfPosition(const Ral_SourceUnit* const source, const int position)
{
	char* c = &source->buffer[position];
	char* positionchar = c;
	int positionchar_offset = 0;

	// Find the start of the line
	while (*c != '\n' && c >= source->buffer)
	{
		c--;
	}
	c++;

	// Find the first non spacer character
	while (*c == '\t' || *c == ' ')
	{
		c++;
	}

	int o = 0;
	while (*c != '\n' && *c != '\0')
	{
		putchar(*c);
		if (c == positionchar) positionchar_offset = o;
		o++;
		c++;
	}

	return positionchar_offset;
}







Ral_List ral_errormessages = { 0 };



void Ral_PushErrorMessage(Ral_ErrorMessage* const errormessage)
{
	Ral_PushFrontList(&ral_errormessages, errormessage);
}



void Ral_PushError_SyntaxErrorPosition(const Ral_SourceUnit* const source, const int position, const int length, const int linenum, const char* const message)
{
	Ral_ErrorMessage* err = Ral_ALLOC_TYPE(Ral_ErrorMessage);
	err->source = source;
	err->position = position;
	err->length = length;
	err->linenum = linenum;
	err->message = message;
	err->type = Ral_ERRMSGTYPE_SYNTAXERROR_POSITION;
	Ral_PushErrorMessage(err);
}





void Ral_PrintErrorMessage(const Ral_ErrorMessage* const errormessage)
{
	if (!errormessage) return;

	switch (errormessage->type)
	{
	case Ral_ERRMSGTYPE_GENERAL:
		printf("> %s\n", errormessage->message);
		break;

	case Ral_ERRMSGTYPE_SYNTAXERROR_POSITION:
		printf("> Syntax error on line %i\n", errormessage->linenum);
		int pos = Ral_PrintLineOfPosition(errormessage->source, errormessage->position);
		putchar('\n');
		for (int i = 0; i < pos; i++)
		{
			putchar(' ');
		}
		if (errormessage->length < 0)
		{
			putchar('^');
		} else
		{
			for (int i = 0; i < errormessage->length; i++)
			{
				putchar('~');
			}
		}
		printf(" - %s\n", errormessage->message);
		break;

	case Ral_ERRMSGTYPE_SYNTAXERROR_TOKEN:
		printf("> Syntax error on line %i\n", errormessage->linenum);

		break;

	default:
		break;
	}
}



void Ral_PrintAllErrorMessages()
{
	// TODO Sort error messages by line number
	printf("ERRORS DETECTED : %i\n", ral_errormessages.itemcount);
	Ral_ErrorMessage* iterator = ral_errormessages.begin;
	while (iterator)
	{
		Ral_PrintErrorMessage(iterator);

		iterator = iterator->next;
	}
	Ral_ClearList(&ral_errormessages, NULL);
}

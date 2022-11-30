#include "ral_logging.h"

#include <stdio.h>
#include "ral_memory.h"



Ral_List ral_errormessages = { 0 };



void Ral_PushErrorMessage(Ral_ErrorMessage* const errormessage)
{
	Ral_PushFrontList(&ral_errormessages, errormessage);
}



void Ral_PushTokenSyntaxError(
	const Ral_Statement* const statement,
	const Ral_Token* const token,
	const char* const message
)
{
	Ral_ErrorMessage* errmsg = Ral_ALLOC_TYPE(Ral_ErrorMessage);
	errmsg->linenum = token->linenum;
	errmsg->position = token->position;
	errmsg->source = statement->parentsource;
	errmsg->token = token;
	errmsg->type = Ral_ERRMSGTYPE_SYNTAXERROR_TOKEN;
	errmsg->message = message;
	Ral_PushErrorMessage(errmsg);
}



void Ral_PrintErrorMessage(const Ral_ErrorMessage* const errormessage)
{
	if (!errormessage) return;

	switch (errormessage->type)
	{
	case Ral_ERRMSGTYPE_GENERAL:
		printf("> %s\n", errormessage->message);
		break;

	case Ral_ERRMSGTYPE_SYNTAXERROR:
		printf("> Syntax error on line %i\n", errormessage->linenum);
		
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
	Ral_ErrorMessage* iterator = ral_errormessages.begin;
	while (iterator)
	{
		Ral_PrintErrorMessage(iterator);

		iterator = iterator->next;
	}
}

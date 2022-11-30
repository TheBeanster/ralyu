#pragma once

#include "ral_stdinclude.h"
#include "ral_list.h"

#include "ral_sourceunit.h"



typedef enum
{
	Ral_ERRMSGTYPE_GENERAL,
	Ral_ERRMSGTYPE_SYNTAXERROR,
	Ral_ERRMSGTYPE_SYNTAXERROR_TOKEN,
} Ral_ErrorMessageType;



typedef struct Ral_ErrorMessage
{
	Ral_LISTLINKS(Ral_ErrorMessage);

	Ral_ErrorMessageType type;

	Ral_SourceUnit* source;
	int position;
	int linenum;

	Ral_Statement* statement;
	Ral_Token* token;

	char* message;
} Ral_ErrorMessage;



void Ral_PushErrorMessage(Ral_ErrorMessage* const errormessage);

void Ral_PushTokenSyntaxError(
	const Ral_Statement* const statement,
	const Ral_Token* const token,
	const char* const message
);

void Ral_PrintErrorMessage(
	const Ral_ErrorMessage* const errormessage
);

void Ral_PrintAllErrorMessages();

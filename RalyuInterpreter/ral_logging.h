#pragma once

#include "ral_stdinclude.h"
#include "ral_list.h"

#include "ral_sourceunit.h"



int Ral_PrintLineOfPosition(
	const Ral_SourceUnit* const source,
	const int position
);



typedef enum
{
	Ral_ERRMSGTYPE_GENERAL,
	Ral_ERRMSGTYPE_SYNTAXERROR_POSITION,
	Ral_ERRMSGTYPE_SYNTAXERROR_TOKEN,
} Ral_ErrorMessageType;



typedef struct Ral_ErrorMessage
{
	Ral_LISTLINKS(Ral_ErrorMessage);

	Ral_ErrorMessageType type;
	
	char* message;

	int linenum;
	Ral_SourceUnit* source;
	int position;
	int length; // The number of chars the error spans. -1 will make it point to a single char

} Ral_ErrorMessage;



void Ral_PushErrorMessage(Ral_SourceUnit* const sourceunit, Ral_ErrorMessage* const errormessage);

void Ral_PushError_SyntaxErrorPosition(
	Ral_SourceUnit* const source,
	const int position,
	const int length,
	const int linenum,
	const char* const message
);





void Ral_PrintErrorMessage(
	const Ral_ErrorMessage* const errormessage
);

void Ral_PrintAllErrorMessages(Ral_SourceUnit* const sourceunit);

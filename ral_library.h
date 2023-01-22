#pragma once

#include "ralu_stdinclude.h"

#include "ral_object.h"
#include "ral_function.h"



typedef enum
{
	Ral_LIBITYPE_END,
	Ral_LIBITYPE_BOOL,
	Ral_LIBITYPE_NUMBER,
	Ral_LIBITYPE_STRING,
	Ral_LIBITYPE_FUNCTION,
} Ral_LibraryIdentifierType;

typedef struct
{
	Ral_LibraryIdentifierType type;
	const char* const name;
	union
	{
		Ral_Bool boolean;
		Ral_Number number;
		const char* const string;
		Ral_CFunction* function;
	};
} Ral_LibraryIdentifier;



void Ral_LoadLibrary(Ral_State* const state, Ral_LibraryIdentifier lib[], const char* const libname);



#define Ral_LoadLib(state, lib) Ral_LoadLibrary(state, lib, #lib)

#define Ral_Library(libname, identifiers) const Ral_LibraryIdentifier libname[] = { identifiers { Ral_LIBITYPE_END } }
#define Ral_LibBool(name, b)			{ Ral_LIBITYPE_BOOL,		name,	.boolean	= b },
#define Ral_LibNumber(name, n)			{ Ral_LIBITYPE_NUMBER,		name,	.number		= n },
#define Ral_LibString(name, s)			{ Ral_LIBITYPE_STRING,		name,	.string		= s },
#define Ral_LibFunction(libname, func)	{ Ral_LIBITYPE_FUNCTION,	#func,	.function	= &libname##func },

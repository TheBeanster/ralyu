#pragma once

#include "ral_stdinclude.h"
#include "ral_list.h"



typedef enum
{
	Ral_TYPETAG_BASE,
	Ral_TYPETAG_STRUCT
} Ral_TypeTag;

typedef struct Ral_Type
{
	Ral_LISTLINKS(Ral_Type);

	char* name;
	Ral_TypeTag tag;
} Ral_Type;





typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	const Ral_Type* type;
	char* name;
} Ral_Object;


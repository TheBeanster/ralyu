#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"
#include "ralu_string.h"



typedef enum
{
	Ral_TYPETAG_INTEGER,
	Ral_TYPETAG_FLOAT,
	Ral_TYPETAG_STRING,
	Ral_TYPETAG_ARRAY
} Ral_TypeTag;

typedef struct Ral_Type
{
	Ral_LISTLINKS(Ral_Type);
	Ral_TypeTag tag;
	char* name;
} Ral_Type;



typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	const Ral_Type* type;
	char* name;

	union
	{
		int integer;
		float number;
		Ral_DynamicString string;
		struct
		{
			Ral_Type* element_type;
			Ral_List elements;
		} arr;
	} value;
} Ral_Object;



Ral_Object* Ral_CreateIntObject(const int i);


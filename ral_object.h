#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"
#include "ralu_string.h"



typedef long double Ral_Number;

typedef enum
{
	Ral_TYPE_NULL,
	Ral_TYPE_NUMBER,
	Ral_TYPE_STRING,
	Ral_TYPE_ARRAY,
	Ral_TYPE_OBJECT,
} Ral_Type;

typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	// The type of the object determines how to interperet the 'value' union
	Ral_Type type;

	union
	{
		Ral_Number number;
		Ral_DynamicString string;
		//Ral_Vector arr;
		struct
		{
			Ral_List members;
		} object;
	} value;
} Ral_Object;



Ral_Object* Ral_CreateNumberObject(const Ral_Number number);
Ral_Object* Ral_CreateStringObject(const char* const string);

void Ral_DestroyObject(Ral_Object* const obj);

Ral_Bool Ral_GetObjectNumber(const Ral_Object* const obj, Ral_Number* const number);
Ral_Bool Ral_GetObjectString(const Ral_Object* const obj, char** const string);

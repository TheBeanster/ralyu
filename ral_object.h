#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"
#include "ralu_string.h"

#include "ral_state.h"
#include "ral_lexer.h"



typedef enum
{
	Ral_TYPETAG_INTEGER,
	Ral_TYPETAG_FLOAT,
	Ral_TYPETAG_STRING,
	Ral_TYPETAG_ARRAY
} Ral_TypeTag;

#define Ral_NUM_BASE_TYPES 3

typedef struct Ral_Type
{
	Ral_LISTLINKS(Ral_Type);
	Ral_TypeTag tag;
	char* name;
	union
	{
		struct Ral_Type* array_element_type;
		Ral_List members;
	};
} Ral_Type;

extern const Ral_Type ral_base_types[];

Ral_Type* Ral_GetTypeS(const Ral_State* const state, const char* const string);

Ral_Type* Ral_GetTypeT(
	const Ral_State* const state,
	const Ral_Token* const tokens,
	const int begin,
	const int end
);





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
			const Ral_Type* element_type;
			Ral_List elements;
		} arr;
	} value;
} Ral_Object;



Ral_Object* Ral_CreateIntObject(
	const int i
);

Ral_Object* Ral_CreateFloatObject(
	const float i
);

Ral_Object* Ral_CreateObject(
	const Ral_Type* const type
);



Ral_Object* Ral_DeclareVariable(
	Ral_List* const scope,
	const Ral_Type* const type,
	const char* const name
);
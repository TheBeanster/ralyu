#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"



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

extern const Ral_Type ral_base_types[];





typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	const Ral_Type* type;
	char* name;
} Ral_Object;

typedef int Ral_Integer;
typedef struct
{
	Ral_Object base;
	Ral_Integer value;
} Ral_Object_Int;

typedef double Ral_Float;
typedef struct
{
	Ral_Object base;
	Ral_Float value;
} Ral_Object_Int;



Ral_Object* Ral_CreateObject(
	const Ral_Type* const type
);
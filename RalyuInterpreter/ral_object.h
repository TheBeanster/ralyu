#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"



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

typedef enum
{
	Ral_BASETYPE_INT,
	Ral_BASETYPE_FLOAT,
} Ral_BaseTypeIndex;

extern const Ral_Type ral_base_types[];

#define Ral_TYPEINT		(&ral_base_types[Ral_BASETYPE_INT])
#define Ral_TYPEFLOAT	(&ral_base_types[Ral_BASETYPE_FLOAT])



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
} Ral_Object_Float;



Ral_Object* Ral_CreateObject(
	const Ral_Type* const type
);

/// @brief Deletes an object. This doesn't account for any other relationships such as list links so make sure to remove them first!
/// @param object 
void Ral_DeleteObject(
	Ral_Object* const object
);



Ral_Object_Int* Ral_CreateIntObjectFromInt(
	const int i
);

Ral_Object_Float* Ral_CreateFloatObjectFromFloat(
	const float f
);

Ral_Object* Ral_CreateObjectFromLiteral(
	const Ral_Token* const token
);

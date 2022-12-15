#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"

#include "ral_lexer.h"



typedef enum
{
	Ral_TYPETAG_BASE,
	Ral_TYPETAG_ARRAY,
	Ral_TYPETAG_STRUCT,
} Ral_TypeTag;

typedef struct Ral_Type
{
	Ral_LISTLINKS(Ral_Type);

	char* name;
	Ral_TypeTag tag;
} Ral_Type;



typedef struct Ral_ArrayType
{
	Ral_Type base;
	const Ral_Type* elementtype;
} Ral_ArrayType;



typedef struct Ral_StructTypeMember
{
	Ral_LISTLINKS(Ral_StructTypeMember);

	char*			name;
	const Ral_Type* type;
} Ral_StructTypeMember;

typedef struct Ral_StructType
{
	Ral_Type base;
	Ral_List* members;
} Ral_StructType;



typedef enum
{
	Ral_BASETYPE_INT,
	Ral_BASETYPE_FLOAT,
	Ral_BASETYPE_STRING,
	Ral_NUMBASETYPES
} Ral_BaseTypeIndex;

extern const Ral_Type ral_base_types[];

#define Ral_TYPEINT		(&ral_base_types[Ral_BASETYPE_INT])
#define Ral_TYPEFLOAT	(&ral_base_types[Ral_BASETYPE_FLOAT])
#define Ral_TYPESTRING	(&ral_base_types[Ral_BASETYPE_STRING])



Ral_ArrayType* Ral_DeclareArrayType(
	struct Ral_State* const state,
	const Ral_Type* const elementtype
);

Ral_StructType* Ral_DeclareStructType(
	struct Ral_State* const state,
	const char* const str,
	const Ral_List* const members
);

Ral_Type* Ral_GetType(
	const struct Ral_State* const state,
	const char* const str
);

Ral_ArrayType* Ral_GetArrayType(
	const struct Ral_State* const state,
	const Ral_Type* const elementtype
);

/// @brief Gets an array type based on the elementtype of an array or creates a new type if it doesn't exist.
/// @param state The state containing the 'arraytypes' list.
/// @param elementtype 
/// @return 
Ral_ArrayType* Ral_GetOrDeclareArrayType(
	struct Ral_State* const state,
	const Ral_Type* const elementtype
);







typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	const Ral_Type* type;
	const char* name;
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
void Ral_DestroyObject(
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

Ral_Object* Ral_CopyObject(
	const Ral_Object* const obj
);

Ral_Object_Int* Ral_CopyIntObject(
	const Ral_Object_Int* const obj
);

Ral_Object_Float* Ral_CopyFloatObject(
	const Ral_Object_Float* const obj
);





Ral_Bool Ral_Object_Assign(Ral_Object* const assign, Ral_Object* const value);

Ral_Object* Ral_Object_Add			(Ral_Object* const a, Ral_Object* const b);
Ral_Object* Ral_Object_Subtract		(Ral_Object* const a, Ral_Object* const b);
Ral_Object* Ral_Object_Multiply		(Ral_Object* const a, Ral_Object* const b);
Ral_Object* Ral_Object_Divide		(Ral_Object* const a, Ral_Object* const b);
Ral_Object* Ral_Object_Negative		(Ral_Object* const a);

Ral_Object* Ral_Object_Equality		(Ral_Object* const a, Ral_Object* const b);
Ral_Object* Ral_Object_Inequality	(Ral_Object* const a, Ral_Object* const b);

Ral_Bool	Ral_Object_IsFalse		(Ral_Object* const a);
Ral_Bool	Ral_Object_IsTrue		(Ral_Object* const a);





/// @brief Creates a new object, assigns a name to it and puts it in 'list'
/// @param list Either the global_variables list in a state or a local_variables list.
/// @param name The name of the variable.
/// @param type The type of the variable.
/// @return A pointer to the new variable.
Ral_Object* Ral_DeclareVariable(
	Ral_List* const			list,
	const char* const		name,
	const Ral_Type* const	type
);

Ral_Object* Ral_GetVariable(
	const struct Ral_State* const	state,
	const Ral_List* const			local_variables,
	const char* const				name
);

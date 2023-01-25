#pragma once

#include "ralu_stdinclude.h"
#include "ralu_list.h"
#include "ralu_string.h"
#include "ralu_vector.h"

#include "ral_state.h"
#include "ral_lexer.h"



typedef long double Ral_Number;

typedef enum
{
	Ral_TYPE_NULL,
	Ral_TYPE_NUMBER,
	Ral_TYPE_BOOL,
	Ral_TYPE_STRING,
	Ral_TYPE_ARRAY,
	Ral_TYPE_STRUCT,
} Ral_Type;

typedef struct Ral_Object
{
	Ral_LISTLINKS(Ral_Object);

	// The type of the object determines how to interperet the 'value' union
	Ral_Type type;

	union
	{
		Ral_Number number;
		Ral_Bool boolean;
		Ral_RString string;
		Ral_Vector arr;
		struct
		{
			Ral_List members; // List of Ral_StructMember
		} struc;
	} val;
} Ral_Object;



typedef struct Ral_StructMember
{
	Ral_LISTLINKS(Ral_StructMember);

	char* name;
	Ral_Object* obj;
} Ral_StructMember;



/// @brief Creates a new object of type 'number'
/// @param number Value to initialize the object
/// @return A pointer to the object
Ral_Object* Ral_CreateNumberObject(const Ral_Number number);

/// @brief Creates a new object of type 'bool'
/// @param boolean Value to initialize the object
/// @return A pointer to the object
Ral_Object* Ral_CreateBoolObject(const Ral_Bool boolean);

/// @brief Creates a new object of type 'string' 
/// @param string Pointer to string to initialize the object. The string will be copied!
/// @return A pointer to the object
Ral_Object* Ral_CreateStringObject(const char* const string);

/// @brief Creates a new empty struct object
/// @return A pointer to the object
Ral_Object* Ral_CreateStructObject();



/// @brief Creates a new object with the same value as 'obj'
/// @param obj Object value to copy
/// @return A pointer to the copy object
Ral_Object* Ral_CopyObject(const Ral_Object* const obj);

/// @brief Clears all memory an object uses except for the object struct and zeroes out the object's data
/// @param obj The object to clear
void Ral_ClearObject(Ral_Object* const obj);

/// @brief Frees an object's memory and all data it uses
/// @param obj Object to free
void Ral_DestroyObject(Ral_Object* const obj);



/// @brief Gets the number value of an object
/// @param obj Object to get the value from
/// @param number Pointer to a Ral_Number where the value will be written
/// @return True if the object is a number and false if it isn't
Ral_Bool Ral_GetObjectNumber(const Ral_Object* const obj, Ral_Number* const number);

/// @brief Gets the string value of an object
/// @param obj Object to get the value from
/// @param number Pointer to a char pointer where the value will be written
/// @return True if the object is a string and false if it isn't
Ral_Bool Ral_GetObjectString(const Ral_Object* const obj, char** const string);



Ral_Bool Ral_AddStructMember(Ral_Object* const obj, const char* const name, const Ral_Object* const val);
Ral_StructMember* Ral_GetStructMemberNode(const Ral_Object* const obj, const char* const name);
Ral_Object* Ral_GetStructMemberValue(const Ral_Object* const obj, const char* const name);
void Ral_RemoveStructMember(Ral_Object* const obj, Ral_StructMember* const member);



Ral_Bool Ral_ObjectIsTrue(const Ral_Object* const obj);



char* Ral_ObjectToString(const Ral_Object* const obj);



Ral_Object* Ral_ObjectAssigmentOperator(
	Ral_State* const state,
	const enum Ral_OperatorID op,
	Ral_Object** assignobj,
	const Ral_Object* const valueobj
);

Ral_Object* Ral_ObjectArithmeticOperator(
	Ral_State* const state,
	const enum Ral_OperatorID op,
	const Ral_Object* const a,
	const Ral_Object* const b
);

Ral_Bool Ral_ObjectRelationalOperator(
	Ral_State* const state,
	const enum Ral_OperatorID op,
	const Ral_Object* const a,
	const Ral_Object* const b
);



void Ral_PrintObjectValue(const Ral_Object* const obj);

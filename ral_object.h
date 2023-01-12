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
	Ral_TYPE_STRUCT,
} Ral_Type;

typedef struct Ral_Object
{
	// The type of the object determines how to interperet the 'value' union
	Ral_Type type;

	union
	{
		Ral_Number number;
		Ral_DynamicString string;
		//Ral_Vector arr;
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
Ral_Object* Ral_GetStructMember(const Ral_Object* const obj, const char* const name);
void Ral_RemoveStructMember(Ral_Object* const obj, Ral_StructMember* const member);



Ral_Object* Ral_ObjectAdd(const Ral_Object* const a, const Ral_Object* const b);
Ral_Object* Ral_ObjectSub(const Ral_Object* const a, const Ral_Object* const b);
Ral_Object* Ral_ObjectMul(const Ral_Object* const a, const Ral_Object* const b);
Ral_Object* Ral_ObjectDiv(const Ral_Object* const a, const Ral_Object* const b);

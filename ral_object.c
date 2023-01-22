#include "ral_object.h"

#include "ralu_memory.h"
#include <stdio.h>


Ral_Object* Ral_CreateNumberObject(const Ral_Number number)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_NUMBER;
    obj->val.number = number;
    return obj;
}



Ral_Object* Ral_CreateBoolObject(const Ral_Bool boolean)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_BOOL;
    obj->val.boolean = boolean;
    return obj;
}



Ral_Object* Ral_CreateStringObject(const char* const string)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_STRING;
    obj->val.string.chars = _strdup(string);
    return obj;
}



Ral_Object* Ral_CopyObject(const Ral_Object* const obj)
{
    if (!obj) return NULL;

    switch (obj->type)
    {
    case Ral_TYPE_NUMBER:
        return Ral_CreateNumberObject(obj->val.number);
        
    case Ral_TYPE_STRING:
        return Ral_CreateStringObject(obj->val.string.chars);

    default:
        break;
    }
}



void Ral_DestroyObject(Ral_Object* const obj)
{
    if (!obj) return;

    switch (obj->type)
    {
    case Ral_TYPE_NUMBER:
        break;
    case Ral_TYPE_STRING:
        
        break;
    default:
        break;
    }

    Ral_FREE(obj);
}



Ral_Bool Ral_GetObjectNumber(const Ral_Object* const obj, Ral_Number* const number)
{
    if (!(obj && number)) return Ral_FALSE;

    if (obj->type != Ral_TYPE_NUMBER) return Ral_FALSE;
    *number = obj->val.number;
    return Ral_TRUE;
}

Ral_Bool Ral_GetObjectString(const Ral_Object* const obj, char** const string)
{
    if (obj->type != Ral_TYPE_STRING) return Ral_FALSE;
    //*string = _strdup(obj->value.string.chars);
    return Ral_TRUE;
}





void Ral_PrintObjectValue(const Ral_Object* const obj)
{
    if (!obj)
    {
        printf("Null object pointer");
        return;
    }

    switch (obj->type)
    {
    case Ral_TYPE_BOOL:
        if (obj->val.boolean)
            printf("true");
        else
            printf("false");
        break;

    case Ral_TYPE_NUMBER:
        printf("%f", obj->val.number);
        break;

    default:
        printf("Invalid object");
        break;
    }
}

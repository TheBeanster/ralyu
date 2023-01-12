#include "ral_object.h"

#include "ralu_memory.h"


Ral_Object* Ral_CreateNumberObject(const Ral_Number number)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_NUMBER;
    obj->value.number = number;
    return obj;
}

Ral_Object* Ral_CreateStringObject(const char* const string)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_STRING;
    //obj->value.string = _strdup(string);
    return obj;
}



void Ral_DestroyObject(Ral_Object* const obj)
{
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
    if (obj->type != Ral_TYPE_NUMBER) return Ral_FALSE;
    *number = obj->value.number;
    return Ral_TRUE;
}

Ral_Bool Ral_GetObjectString(const Ral_Object* const obj, char** const string)
{
    if (obj->type != Ral_TYPE_STRING) return Ral_FALSE;
    //*string = _strdup(obj->value.string.chars);
    return Ral_TRUE;
}

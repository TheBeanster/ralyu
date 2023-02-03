#include "ral_object.h"

#include "ralu_memory.h"
#include <stdio.h>
#include <math.h>



Ral_Object* Ral_CreateNumberObject(const Ral_Number number)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_NUMBER;
    obj->vnumber = number;
    return obj;
}



Ral_Object* Ral_CreateBoolObject(const Ral_Bool boolean)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_BOOL;
    obj->vboolean = boolean;
    return obj;
}



Ral_Object* Ral_CreateStringObject(const char* const string)
{
    Ral_Object* obj = Ral_ALLOC_TYPE(Ral_Object);
    obj->type = Ral_TYPE_STRING;
    obj->vstring.chars = _strdup(string);
    return obj;
}



Ral_Object* Ral_CopyObject(const Ral_Object* const obj)
{
    if (!obj) return NULL;

    switch (obj->type)
    {
    case Ral_TYPE_NUMBER:
        return Ral_CreateNumberObject(obj->vnumber);
        
    case Ral_TYPE_STRING:
        return Ral_CreateStringObject(obj->vstring.chars);

    default:
        return NULL;
        break;
    }
}



void Ral_ClearObject(Ral_Object* const obj)
{
    if (!obj) return;

    switch (obj->type)
    {
    case Ral_TYPE_STRUCT:

        break;
    default:
        break;
    }
}



void Ral_DestroyObject(Ral_Object* const obj)
{
    if (!obj) return;
    Ral_ClearObject(obj);
    Ral_FREE(obj);
}



Ral_Bool Ral_GetObjectNumber(const Ral_Object* const obj, Ral_Number* const number)
{
    if (!(obj && number)) return Ral_FALSE;

    if (obj->type != Ral_TYPE_NUMBER) return Ral_FALSE;
    *number = obj->vnumber;
    return Ral_TRUE;
}

Ral_Bool Ral_GetObjectString(const Ral_Object* const obj, char** const string)
{
    if (obj->type != Ral_TYPE_STRING) return Ral_FALSE;
    //*string = _strdup(obj->value.string.chars);
    return Ral_TRUE;
}





char* Ral_ObjectToString(const Ral_Object* const obj)
{
    if (!obj) return NULL;
    
    switch (obj->type)
    {
    case Ral_TYPE_BOOL:
        if (obj->vboolean)
            return _strdup("true");
        else
            return _strdup("false");

    case Ral_TYPE_NUMBER:
    {
        char str[200];
        if (floor(obj->vnumber) == obj->vnumber)
        {
            // Number is whole
            snprintf(str, 200, "%ll", (long long)obj->vnumber);
        } else
        {
            // Number has fractional part
            snprintf(str, 200, "%f", obj->vnumber);
        }
        
        return _strdup(str);
    }

    default:
        break;
    }
    return NULL;
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
        if (obj->vboolean)
            printf("true");
        else
            printf("false");
        break;

    case Ral_TYPE_NUMBER:
        if (floor(obj->vnumber) == obj->vnumber)
            printf("%d", (int)obj->vnumber);
        else
            printf("%f", obj->vnumber);
        break;

    default:
        printf("Invalid object");
        break;
    }
}

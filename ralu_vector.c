#include "ralu_vector.h"

#include "ralu_memory.h"



void Ral_InitEmptyVector(Ral_Vector* const v)
{
    v->items = Ral_CALLOC(4, sizeof(void*));
    v->itemcount = 0;
    v->capacity = 4;
}



Ral_Bool Ral_ResizeVector(Ral_Vector* const v, const int capacity)
{
    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items)
    {
        v->items = items;
        v->capacity = capacity;
    } else return Ral_FALSE;
    return Ral_TRUE;
}



void Ral_PushBackVector(Ral_Vector* const v, const void* const item)
{
    if (v->capacity == v->itemcount)
    {
        if (Ral_ResizeVector(v, v->capacity * 2))
        {
            v->items[v->itemcount++] = item;
        } else return Ral_FALSE;
    }
    else
    {
        v->items[v->itemcount++] = item;
    }
    return Ral_TRUE;
}



void* Ral_VectorGet(Ral_Vector* const v, const int index)
{
    if ((index >= 0) && (index < v->itemcount))
    {
        return v->items[index];
    }
    return NULL;
}



Ral_Bool Ral_VectorRemove(Ral_Vector* const v, const int index)
{
    int i = 0;
    if ((index < 0) || (index >= v->itemcount))
        return Ral_FALSE;

    v->items[index] = NULL;
    for (i = index; (i < v->itemcount - 1); ++i)
    {
        v->items[i] = v->items[i + 1];
        v->items[i + 1] = NULL;
    }
    v->itemcount--;
    if ((v->itemcount > 0) && ((v->itemcount) <= (v->capacity / 4)))
    {
        Ral_ResizeVector(v, v->capacity / 2);
    }
    return Ral_TRUE;
}



void Ral_ClearVector(Ral_Vector* const v, void(*destroy_func)(void*))
{
    for (int i = 0; i < v->itemcount; i++)
    {
        if (destroy_func)
            destroy_func(v->items[i]);
        else
            Ral_FREE(v->items[i]);
    }
    Ral_FREE(v->items);
    Ral_InitEmptyVector(v);
}

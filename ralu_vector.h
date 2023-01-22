#pragma once

#include "ralu_stdinclude.h"



#define Ral_VECTOR_EMPTY_CAPACITY 4

typedef struct
{
	void** items;
	int itemcount;
	int capacity;
} Ral_Vector;



void Ral_InitEmptyVector(Ral_Vector* const v);

Ral_Bool Ral_ResizeVector(Ral_Vector* const v, const int capacity);

void Ral_PushBackVector(Ral_Vector* const v, const void* const item);

void* Ral_VectorGet(Ral_Vector* const v, const int index);

Ral_Bool Ral_VectorRemove(Ral_Vector* const v, const int index);

void Ral_ClearVector(Ral_Vector* const v, void(*destroy_func)(void*));

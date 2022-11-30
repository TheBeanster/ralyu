#pragma once

#include "ral_stdinclude.h"



#define Ral_LISTLINKS(type) struct type* prev; struct type* next



typedef struct Ral_Listnode
{
	struct Ral_Listnode* prev;
	struct Ral_Listnode* next;
} Ral_Listnode;



typedef struct Ral_List
{
	Ral_Listnode* begin;
	Ral_Listnode* end;
	int itemcount;
} Ral_List;



void Ral_PushFrontList(
	Ral_List* const		list,
	Ral_Listnode* const	node
);

void* Ral_PopFrontList(
	Ral_List* const		list
);



void Ral_UnlinkFromList(
	Ral_List* const		list,
	Ral_Listnode* const	node
);



/// <summary>
/// Empties a list of all elements. Doesn't free the list struct itself!
/// </summary>
/// <param name="list">Pointer to the list</param>
/// <param name="destroy_func">Pointer to destroy function of the type in the list. Set to NULL if not needed</param>
void Ral_ClearList(
	Ral_List* const		list,
	void (*destroy_func)(void*)
);

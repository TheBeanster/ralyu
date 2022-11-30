#include "ral_list.h"

#include "ral_memory.h"



void Ral_PushFrontList(
	Ral_List* const		list,
	Ral_Listnode* const	node
)
{
	if (list->begin == NULL)
	{
		list->begin = node;
		list->end = node;
	} else
	{
		list->end->next = node;
	}

	node->prev = list->end;
	list->end = node;
	node->next = NULL;


	list->itemcount++;
}



void* Ral_PopFrontList(
	Ral_List* const list
)
{
	if (!list->begin) return NULL;

	Ral_Listnode* node = list->end;

	if (node == list->begin)
	{
		list->begin = NULL;
		list->end = NULL;
	}



	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;

	if (list->end)
		list->end = node->prev;



	list->itemcount--;

	return node;
}



void Ral_UnlinkFromList(Ral_List* const list, Ral_Listnode* const node)
{
	if (node->prev == NULL)
	{
		if (node->next == NULL)
		{
			list->begin = NULL;
			list->end = NULL;
		} else
		{
			node->next->prev = NULL;
			list->begin = node->next;
		}
	} else
	{
		if (node->next == NULL)
		{
			node->prev->next = NULL;
			list->end = node->prev;
		} else
		{
			node->next->prev = node->prev;
			node->prev->next = node->next;
		}
	}
}



void Ral_ClearList(Ral_List* const list, void(*destroy_func)(void*))
{
	Ral_Listnode* iterator = list->begin;
	while (iterator)
	{
		Ral_Listnode* delblock = iterator;
		iterator = iterator->next;
		if (destroy_func)
			destroy_func(delblock);
		else
			Ral_FREE(delblock);
	}
}

#include "ralu_list.h"



void Ral_PushFrontList(
	Ral_List* const		list,
	Ral_ListNode* const	node
)
{
	if (list->begin == NULL)
	{
		// List is empty
		node->prev = NULL;
		node->next = NULL;
		list->begin = node;
		list->end = node;
	} else
	{
		node->prev = NULL;
		node->next = list->begin;
		list->begin->prev = node;
		list->begin = node;
	}
	list->itemcount++;
}



void Ral_PushBackList(
	Ral_List* const		list,
	Ral_ListNode* const	node
)
{
	if (list->begin == NULL)
	{
		// List is empty
		node->prev = NULL;
		node->next = NULL;
		list->begin = node;
		list->end = node;
	} else
	{
		node->prev = list->end;
		node->next = NULL;
		list->end->next = node;
		list->end = node;
	}
	list->itemcount++;
}



void* Ral_PopFrontList(Ral_List* const list)
{
	Ral_ListNode* node = list->begin;
	Ral_UnlinkFromList(list, node);
	return node;
}



void* Ral_PopBackList(Ral_List* const list)
{
	Ral_ListNode* node = list->end;
	Ral_UnlinkFromList(list, node);
	return node;
}



void Ral_UnlinkFromList(
	Ral_List* const list,
	Ral_ListNode* const node
)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		list->begin = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		list->end = node->prev;

	node->prev = NULL;
	node->next = NULL;

	list->itemcount--;
}

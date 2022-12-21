#include "ral_utility.h"



void Ral_PushFrontList(
	Ral_List* const		list,
	Ral_ListNode* const	node
)
{
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	} else
	{
		list->head->prev = node;
	}
	node->prev = NULL;
	node->next = list->head;
	list->head = node;
	list->itemcount++;
}



void Ral_PushBackList(
	Ral_List* const		list,
	Ral_ListNode* const	node
)
{
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	} else
	{
		list->tail->next = node;
	}
	node->prev = list->tail;
	node->next = NULL;
	list->tail = node;
	list->itemcount++;
}




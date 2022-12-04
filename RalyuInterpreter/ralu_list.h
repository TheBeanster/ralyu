#pragma once

#include "ralu_stdinclude.h"



#define Ral_LISTLINKS(type) struct type* prev; struct type* next



typedef struct Ral_Listnode
{
	struct Ral_Listnode* prev;
	struct Ral_Listnode* next;
} Ral_Listnode;



/// @brief A doubly linked list keeping track of the start and end items and the number of items in it.
typedef struct Ral_List
{
	Ral_Listnode* begin;
	Ral_Listnode* end;
	int itemcount;
} Ral_List;



/// @brief Pushes an element onto the front of a list.
/// @param list The list to push the element onto.
/// @param node Pointer to the element to push.
void Ral_PushFrontList(
	Ral_List* const		list,
	Ral_Listnode* const	node
);

/// @brief Removes the element at the front of the list and returns a pointer to it.
/// @param list The list to get the element from.
/// @return A pointer to the element or NULL if the list was empty.
void* Ral_PopFrontList(
	Ral_List* const		list
);



/// @brief Removes a nodes from a list and links the surrounding elements together.
/// @param list Pointer to the parent list.
/// @param node The node to unlink from the list.
void Ral_UnlinkFromList(
	Ral_List* const		list,
	Ral_Listnode* const	node
);



/// @brief Removes all elements from a list and calling the destroy_func on every element.
/// This does not free the list itself, only empties it!
/// @param list Pointer to the list to empty.
/// @param destroy_func Pointer to a destructor function, if NULL it will use Ral_FREE().
void Ral_ClearList(
	Ral_List* const		list,
	void (*destroy_func)(void*)
);

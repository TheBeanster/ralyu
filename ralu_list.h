#pragma once

#include "ralu_stdinclude.h"



// Ralyu linked list implementation



typedef struct Ral_ListNode
{
	struct Ral_ListNode* prev;
	struct Ral_ListNode* next;
} Ral_ListNode;

// Doubly linked list
typedef struct Ral_List
{
	Ral_ListNode* begin;
	Ral_ListNode* end;
	int itemcount;
} Ral_List;

// Header used to allow a type to be put in linked lists. Has prev and next pointers
// for the list links. This must be the first member of a struct to cast it directly
// to a Ral_ListNode type. Otherwise, you must specify where these links are.
#define Ral_LISTLINKS(type) struct type* prev; struct type* next



/// @brief Pushes an element onto the beginning of a list.
/// @param list The list to push the element onto.
/// @param node Pointer to the element to push. (Type must have the Ral_LISTLINKS() header!)
void Ral_PushFrontList(
	Ral_List* const		list,
	Ral_ListNode* const	node
);

/// @brief Pushes an element onto the end of a list.
/// @param list The list to push the element onto.
/// @param node Pointer to the element to push. (Type must have the Ral_LISTLINKS() header!)
void Ral_PushBackList(
	Ral_List* const		list,
	Ral_ListNode* const	node
);

/// @brief Removes the element at the beginning of the list and returns a pointer to it.
/// @param list The list to get the element from.
/// @return A pointer to the element or NULL if the list was empty. Make sure to know
/// the type of the element returned to cast it from the void* returned.
void* Ral_PopFrontList(
	Ral_List* const		list
);

/// @brief Removes the element at the end of the list and returns a pointer to it.
/// @param list The list to get the element from.
/// @return A pointer to the element or NULL if the list was empty. Make sure to know
/// the type of the element returned to cast it from the void* returned.
void* Ral_PopBackList(
	Ral_List* const		list
);

/// @brief Removes a node from a list and links the surrounding elements together.
/// @param list The list to unlink from.
/// @param node The node to unlink from the list.
void Ral_UnlinkFromList(
	Ral_List* const		list,
	Ral_ListNode* const	node
);
#pragma once

#include "ral_stdinclude.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <ctype.h>





// Ralyu memory management utilities

// Compile memory management functions with raw standard library function calls
// or call specific ralyu implementations with error checking.
// Compiling the normal standard library functions could be useful in some IDEs
// that keep track of allocations by checking these.
#define Ral_COMPILE_USE_STD_MALLOC 1

#if Ral_COMPILE_USE_STD_MALLOC

#define Ral_MALLOC(size)			malloc(size)
#define Ral_CALLOC(count, size)		calloc(count, size)
#define Ral_REALLOC(block, size)	realloc(block, size)
#define	Ral_ALLOC_TYPE(type)		(type*)calloc(1, sizeof(type))
#define Ral_FREE(block)				free(block)

#else

void* Ral_Malloc(const size_t size);
void* Ral_Calloc(const int count, const size_t size);
void* Ral_Realloc(void* block, const size_t size);
void* Ral_Free(const void* const block);

#define Ral_MALLOC(size)			Ral_Malloc(size)
#define Ral_CALLOC(count, size)		Ral_Calloc(count, size)
#define	Ral_ALLOC_TYPE(type)		Ral_Calloc(1, sizeof(type))
#define Ral_FREE(block)				Ral_Free(block)

#endif





// Ralyu linked list implementation

typedef struct Ral_ListNode
{
	struct Ral_ListNode* prev;
	struct Ral_ListNode* next;
} Ral_ListNode;

// Doubly linked list
typedef struct Ral_List
{
	Ral_ListNode* head;
	Ral_ListNode* tail;
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

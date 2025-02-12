#include "../include/linkedlist.h"
#include "../include/node.h"
#include <string.h>

/**
 * Struct that represent a node that can store
 * a generic type value
 *
 * Used in Linked Lists and Circular Linked List
 *
 * The node itself has two components, a value stored, and
 * a pointer to the next node in the list
 */
typedef struct node {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	 /* Pointer to the memory location that stores the node's value
	  *
	  * Void* is used to accept any type of data
	  */
	void* ptr;

	/* Pointer to the next node 
	 * (as in next in a list)
	 */
	node* next;
} node;

/**
 * Struct that represent a list of elements of a generic type value
 */
typedef struct linkedlist {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Pointer to the first node (head) of the list */
	node* head;

	/* Number of elements present in the list */
	size_t element_count;

	/* Size of the elements stored in the list */
	size_t element_size;

} linkedlist;

/**
 *  Creates a linked list ready to store elements that are as big as the given size
 */
linkedlist* ll_create(size_t element_size);

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void ll_delete(linkedlist** ll);

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void ll_insert_at(linkedlist* ll, void* x, size_t i);

/**
 * Insert the element pointed to by x as the new head of the list
 */
void ll_insert_head(linkedlist* ll, void* x);

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void ll_insert_tail(linkedlist* ll, void* x);

/**
 * Removes the i -th element from the list
 */
void ll_remove_at(linkedlist* ll, size_t i);

/**
 * Removes the head from the list
 */
void ll_remove_head(linkedlist* ll);

/**
 * Removes the tail from the list
 */
void ll_remove_tail(linkedlist* ll);

/**
 * Returns a pointer to the i -th element of the list
 */
void* ll_get_at(linkedlist* ll, size_t i);

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_at(linkedlist* ll, size_t i, void* buf);

/**
 * Returns a pointer to the head of the list
 */
void* ll_get_head(linkedlist* ll);

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_head(linkedlist* ll, void* buf);

/**
 * Returns a pointer to the tail of the list
 */
void* ll_get_tail(linkedlist* ll);

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_tail(linkedlist* ll, void* buf);

/**
 * Returns the number of elements of the list
 */
size_t ll_get_size(linkedlist* ll);

/**
 * Returns the size of the elements of the list
 */
size_t ll_get_element_size(linkedlist* ll);

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to ll_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short ll_contains(linkedlist* ll, void* x);

/**
 * Checks whether the list contains at least one element or not
 */
bool ll_is_empty(linkedlist* ll);

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void ll_clear(linkedlist* ll);
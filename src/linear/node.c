/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/node.h"
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
 * Creates a node that will store the content pointed to by 'value'
 * That will be written in 'value_size' bytes
 *
 * eg. to store an int, value_size = sizeof(int)
 */
node* node_create(void* value, size_t value_size) {

	node* n = NULL;

	// Check if the size is reasonable
	if (value_size <= SIZE_MAX) {

		// Allocate memory for the node struct
		n = (node*)malloc(sizeof(node));

		// If it was allocated
		if (n) {

			// Allocate memory for the value
			n->ptr = malloc(value_size);

			// If it was allocated
			if (n->ptr) {

				// Initialize the fields
				memcpy(n->ptr, value, value_size);
				n->next = NULL;
			}

			// Otherwise
			else {

				// Cancel the creation, free the memory used for the struct
				free(n);
				n = NULL;
			}
		}
	}

	return n;
}

/**
 * Deletes the given node
 *
 * It frees the memory used to store the value, then
 * the memory used for the struct itself
 */
void node_delete(node** n) {

	// Check if the ptr fields needs to be freed as well
	if (n != NULL && *n != NULL) {

		// Free the memory used for the value
		free((*n)->ptr);

		// Free the memory used for the struct
		memset(*n, 0, sizeof(node));
		free(*n);
		*n = NULL;
	}
	return;
}

/**
 * Returns a pointer to the value stored in the
 * node
 *
 * Useful for returning structs, without
 * the need to copy all of its content
 */
void* node_get_value(node* n) {

	return n ? n->ptr : NULL;
}

/**
 * Returns a pointer to the next node, that is
 * the node pointed to by this one
 */
node* node_get_next(node* n) {

	node* next = NULL;

	// Integrity check
	if (n) {
		next = n->next;
	}

	return next;
}

/**
 * Sets 'next' as the next node, that is the
 * node pointed to by this one
 */
void node_set_next(node* n, node* next) {

	// Integrity check
	if (n) {
		n->next = next;
	}

	return;
}
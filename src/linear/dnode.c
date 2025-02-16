/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/dnode.h"
#include <string.h>

/**
 * Struct that represent a node that can store
 * a generic type value, and has pointers
 * to both the previous and next element
 *
 * Used in Double Linked Lists and Double Circular Linked Lists
 *
 * The node itself has three components, a value stored,
 * a pointer to the next node in the list, and a pointer
 * to the previous node
 */
typedef struct dnode {

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

	/* Pointer to the previous node
	 * (as in previous in a list)
	 */
	dnode* prev;

	/* Pointer to the next node 
	 * (as in next in a list)
	 */
	dnode* next;
} dnode;

/**
 * Creates a node that will store the content pointed to by 'value'
 * That will be written in 'value_size' bytes
 *
 * eg. to store an int, value_size = sizeof(int)
 */
dnode* dnode_create(void* value, size_t value_size) {

	dnode* dn = NULL;

	// Check if the size is reasonable
	if (value_size <= SIZE_MAX) {

		// Allocate memory for the node struct
		dn = (dnode*)malloc(sizeof(dnode));

		// If it was allocated
		if (dn) {

			// Allocate memory for the value
			dn->ptr = malloc(value_size);

			// If it was allocated
			if (dn->ptr) {

				// Initialize the fields
				memcpy(dn->ptr, value, value_size);
				dn->prev = NULL;
				dn->next = NULL;
			}

			// Otherwise
			else {

				// Cancel the creation, free the memory used for the struct
				free(dn);
				dn = NULL;
			}
		}
	}
	return dn;
}

/**
 * Deletes the given node
 *
 * It frees the memory used to store the value, then
 * the memory used for the struct itself
 */
void dnode_delete(dnode** dn) {

	// Check if the ptr fields needs to be freed as well
	if (dn != NULL && *dn != NULL) {

		// Free the memory used for the value
		free((*dn)->ptr);

		// Free the memory used for the struct
		memset(*dn, 0, sizeof(dnode));
		free(*dn);
		*dn = NULL;
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
void* dnode_get_value(dnode* dn) {

	return dn ? dn->ptr : NULL;
}

/**
 * Returns a pointer to the next node
 */
dnode* dnode_get_next(dnode* dn) {

	return dn ? dn->next : NULL;
}

/**
 * Sets 'next' as the next node
 */
void dnode_set_next(dnode* dn, dnode* next) {

	if (dn) dn->next = next;
}

/**
 * Returns a pointer to the previous node
 */
dnode* dnode_get_prev(dnode* dn) {

	return dn ? dn->prev : NULL;
}

/**
 * Sets 'prev' as the previous node
 */
void dnode_set_prev(dnode* dn, dnode* prev) {

	if (dn) dn->prev = prev;
}
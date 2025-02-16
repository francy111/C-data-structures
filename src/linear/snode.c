/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/snode.h"
#include <string.h>

/**
 * Struct that represent a node that can store
 * a generic type value
 *
 * Used in Skip Lists
 *
 * The node itself has two components, a value stored, and
 * an array of pointers to the nodes at the next levels
 */
typedef struct snode {

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

	/* Array of pointers to the next nodes of the successive levels */
	snode** forward;

	/* Level the node has inside the list it is part of */
	size_t level;
} snode;

/**
 * Creates a node that will store the content pointed to by 'value'
 * That will be written in 'value_size' bytes
 *
 * eg. to store an int, value_size = sizeof(int)
 */
snode* snode_create(void* value, size_t value_size, size_t level) {

	snode* sn = NULL;

	// Check if the size is reasonable
	if (0 < value_size && value_size <= SIZE_MAX && 0 < level && level <= SIZE_MAX) {

		// Allocate memory for the node struct
		sn = (snode*)malloc(sizeof(snode));

		// If it was allocated
		if (sn) {

			// Allocate memory for the value
			sn->ptr = malloc(value_size);
			sn->level = level;

			// If it was allocated
			if (sn->ptr) {

				// Initialize the fields
				memcpy(sn->ptr, value, value_size);

				sn->forward = (snode**)malloc(level * sizeof(snode*));
				for (int i = 0; i < sn->level; i++) {

					sn->forward[i] = (snode*)NULL;
				}

				// Cancel the creation
				if (!sn->forward) {

					free(sn->ptr);
					free(sn);
					sn = NULL;
				}
			}

			// Otherwise
			else {

				// Cancel the creation, free the memory used for the struct
				free(sn);
				sn = NULL;
			}
		}
	}
	return sn;
}

/**
 * Deletes the given node
 *
 * It frees the memory used to store the value, then
 * the memory used for the struct itself
 */
void snode_delete(snode** sn) {

	if (sn && *sn) {

		// Free the memory used to store the element
		free((*sn)->ptr);

		// Free the memory used for the pointers
		free((*sn)->forward);

		// Free the memory used for the struct itself
		free(*sn);
		*sn = NULL;
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
void* snode_get_value(snode* sn) {

	return sn ? sn->ptr : NULL;
}

/**
 * Returns a pointer to the next node of the given level, that is
 * the node pointed to by this one
 */
snode* snode_get_next(snode* sn, size_t level) {

	// Return pointer to the next node at that particular level, a node with n levels will have pointers that go from level 0 (forward[0] to level n - 1 (forward[n - 1])
	return (level < sn->level) ? sn->forward[level] : NULL;
}

/**
 * Sets 'next' as the next node in the given level, that is the
 * node pointed to by this one
 */
void snode_set_next(snode* sn, snode* next, size_t level) {

	if (level < sn->level) sn->forward[level] = next;
	return;
}

/**
 * Returns the level of the node, inside the list it is part of
 */
size_t snode_get_level(snode* sn) {

	return sn ? sn->level : -1;
}

/**
 * Sets the level of the node inside the list it is part of
 */
void snode_set_level(snode* sn, size_t level) {

	if (sn) sn->level = level;
	return;
}
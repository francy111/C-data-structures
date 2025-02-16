/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/linkedlist.h"
#include "../../include/linear/node.h"
#include <string.h>

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
linkedlist* ll_create(size_t element_size) {

	linkedlist* ll = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		ll = (linkedlist*)malloc(sizeof(linkedlist));

		if (ll) {

			ll->element_size = element_size;
			ll->element_count = 0;
		}
	}
	return ll;
}

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void ll_delete(linkedlist** ll) {

	// Access the list only if the pointer is valid
	if (ll && *ll) {

		// Free every node
		ll_clear(*ll);

		// Free the memory used for the whole struct
		memset(*ll, 0, sizeof(linkedlist));
		free(*ll);
		*ll = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void ll_insert_at(linkedlist* ll, void* x, size_t i) {

	// Check for pointer validity
	if (ll && x) {
		
		// Check if the position is correct
		if (i <= ll->element_count) {

			// Create the node
			node* n = node_create(x, ll->element_size);

			// Continue only if the node was created
			if (n) {

				// Since we need to iterate to the (i-1) -th element, the case i = 0 has to be handled differently
				if (i == 0) {

					node_set_next(n, ll->head); // Connect the new node with the old head
					ll->head = n; // And replace the list's head
				}

				// In every other case
				else {

					node* tmp = ll->head;

					// Iterate to the element before i
					for (int j = 1; j < i; j++) {

						tmp = node_get_next(tmp);
					}

					// Connect the node n between the node i-1 and i, becoming the new i -th node
					node_set_next(n, node_get_next(tmp));
					node_set_next(tmp, n);
				}

				ll->element_count++;
			}
		}
	}
	return;
}

/**
 * Insert the element pointed to by x as the new head of the list
 */
void ll_insert_head(linkedlist* ll, void* x) {

	// Check for pointer validity
	if (ll && x) {

		ll_insert_at(ll, x, 0);
	}
	return;
}

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void ll_insert_tail(linkedlist* ll, void* x) {

	// Check for pointer validity
	if (ll && x) {

		ll_insert_at(ll, x, ll->element_count);
	}
	return;
}

/**
 * Removes the i -th element from the list
 */
void ll_remove_at(linkedlist* ll, size_t i) {

	// Check for pointer validity
	if (ll) {

		// Check if the position is correct
		if (i < ll->element_count) {

			node* to_be_deleted = NULL;

			// Since we need to iterate to the (i-1) -th element, the case i = 0 has to be handled differently
			if (i == 0) {
				
				// Remove the head
				to_be_deleted = ll->head;

				// Logical removal, shift the head
				ll->head = node_get_next(ll->head);
			}

			// In every other case
			else {

				node* tmp = ll->head;

				// Iterate to the element before i
				for (int j = 1; j < i; j++) {

					tmp = node_get_next(tmp);
				}

				// Logically remove the node first, then physically
				to_be_deleted = node_get_next(tmp);
				
				// Logical removal, connect the node i-1 to i+1
				node_set_next(tmp, node_get_next(node_get_next(tmp)));
			}

			node_delete(&to_be_deleted);
			ll->element_count--;
		}
	}
	return;
}

/**
 * Removes the head from the list
 */
void ll_remove_head(linkedlist* ll) {

	// Check for pointer validity
	if (ll) {

		ll_remove_at(ll, 0);
	}
	return;
}

/**
 * Removes the tail from the list
 */
void ll_remove_tail(linkedlist* ll) {

	// Check for pointer validity
	if (ll) {

		ll_remove_at(ll, ll->element_count - 1);
	}
	return;
}

/**
 * Returns a pointer to the i -th element of the list
 */
void* ll_get_at(linkedlist* ll, size_t i) {

	void* ret = NULL;

	// Check for pointer validity
	if (ll) {

		// Check if the position is correct
		if (i < ll->element_count) {

			// Iterate to the i -th node
			node* tmp = ll->head;

			for (int j = 0; j < i; j++) {

				tmp = node_get_next(tmp);
			}
			
			// Return the value
			ret = node_get_value(tmp);
		}
	}
	return ret;
}

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_at(linkedlist* ll, size_t i, void* buf) {

	// Check for pointer validity
	if (ll) {

		// Check if the position is correct
		if (i < ll->element_count) {

			// Iterate to the i -th node
			node* tmp = ll->head;

			for (int j = 0; j < i; j++) {

				tmp = node_get_next(tmp);
			}

			// Copy the value
			memcpy(buf, node_get_value(tmp), ll->element_size);
		}
	}
	return;
}

/**
 * Returns a pointer to the head of the list
 */
void* ll_get_head(linkedlist* ll) {

	return ll ? ll_get_at(ll, 0) : NULL;
}

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_head(linkedlist* ll, void* buf) {

	ll_get_2_at(ll, 0, buf);
	return;
}

/**
 * Returns a pointer to the tail of the list
 */
void* ll_get_tail(linkedlist* ll) {

	return ll ? ll_get_at(ll, ll->element_count - 1) : NULL;
}

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_tail(linkedlist* ll, void* buf) {

	ll_get_2_at(ll, ll->element_count - 1, buf);
	return;
}

/**
 * Returns the number of elements of the list
 */
size_t ll_get_size(linkedlist* ll) {

	return ll ? ll->element_count : 0;
}

/**
 * Returns the size of the elements of the list
 */
size_t ll_get_element_size(linkedlist* ll) {

	return ll ? ll->element_size : 0;
}

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to ll_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short ll_contains(linkedlist* ll, void* x) {

	int isPresent = 0, index = 0;

	// Check for pointer validity
	if (ll && x) {

		// Iterate through the list and compare with each element
		node* tmp = ll->head;
		for (index = 0; index < ll->element_count; index++) {

			// Compare the i -th element with the element pointed to by x
			isPresent = (memcmp(x, node_get_value(tmp), ll->element_size) == 0);
			if (isPresent)
				break;
			tmp = node_get_next(tmp);
		}
	}
	return isPresent ? index + 1 : 0;
}

/**
 * Checks whether the list contains at least one element or not
 */
bool ll_is_empty(linkedlist* ll) {

	return ll ? !(ll->element_count) : false;
}

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void ll_clear(linkedlist* ll) {

	// Access the list if the pointer is valid
	if (ll) {

		node* to_be_deleted = NULL;
		node* tmp = ll->head;
		for (int i = 0; i < ll->element_count; i++) {

			to_be_deleted = tmp;
			tmp = node_get_next(tmp);
			node_delete(&to_be_deleted);
		}
		ll->head = NULL;
		ll->element_count = 0;
	}
}

/**
 * Applies the function f to every element
 * of the linked list ll
 */
void ll_for_each(linkedlist* ll, void (*f)(void*)) {

	// Parameters check
	if (ll && f) {

		// Iterate the list, applying the function to each element
		node* tmp = ll->head;
		void* tmp_buf = malloc(ll->element_size);
		if (tmp_buf) {

			// Apply the function to each element
			for (int i = 0; i < ll->element_count; i++) {

				// Get a copy of the element for safety reasons
				memcpy(tmp_buf, node_get_value(tmp), ll->element_size);

				// Apply the function to the copy
				f(tmp_buf);
				tmp = node_get_next(tmp);
			}
			free(tmp_buf);
		}
	}
	return;
}

/**
 * Returns a linked list obtained by applying
 * the function f to every element of the original list ll
 */
linkedlist* ll_map(linkedlist* ll, void* (*f)(void*)) {

	linkedlist* mapped = NULL;

	// Parameters check
	if (ll && f) {

		mapped = ll_create(ll->element_size);
		if (mapped) {

			// Iterate the list
			node* tmp = ll->head;
			void* tmp_buf = malloc(ll->element_size);
			if (tmp_buf) {

				// Apply the function to each element and insert it in the new list
				for (int i = 0; i < ll->element_count; i++) {

					// Get a copy of the element for safety reasons
					memcpy(tmp_buf, node_get_value(tmp), ll->element_size);

					// Apply the function to the copy and insert it in the list
					ll_insert_at(mapped, f(tmp_buf), i);
					tmp = node_get_next(tmp);
				}
				free(tmp_buf);
			}
		}
	}
	return mapped;
}
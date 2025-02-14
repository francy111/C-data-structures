/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/clinkedlist.h"
#include "../include/node.h"
#include <string.h>

/**
 * Struct that represent a circular list of elements of a generic type value
 */
typedef struct clinkedlist {

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

} clinkedlist;

/**
 *  Creates a circular linked list ready to store elements that are as big as the given size
 */
clinkedlist* cl_create(size_t element_size) {

	clinkedlist* cl = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		cl = (clinkedlist*)malloc(sizeof(clinkedlist));

		if (cl) {

			cl->element_size = element_size;
			cl->element_count = 0;
		}
	}
	return cl;
}

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void cl_delete(clinkedlist** cl) {

	// Access the list only if the pointer is valid
	if (cl && *cl) {

		// Free every node
		cl_clear(*cl);

		// Free the memory used for the whole struct
		memset(*cl, 0, sizeof(clinkedlist));
		free(*cl);
		*cl = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void cl_insert_at(clinkedlist* cl, void* x, size_t i) {

	// Check for pointer validity
	if (cl && x) {
		
		// Check if the position is correct
		if (i <= cl->element_count) {

			// Create the node
			node* n = node_create(x, cl->element_size);

			// Continue only if the node was created
			if (n) {

				/* Since we need to iterate to the(i - 1) - th element, the one before the first is the last
				 * The cases insertTail and insertHead are the same, however, only when insearting a new head we need
				 * to update the head pointer
				 */

				node* tmp = cl->head;
				size_t actual_index = (i == 0) ? cl->element_count : i;

				if (actual_index) {

					// Iterate to the element before i
					for (int j = 1; j < actual_index; j++) {
						tmp = node_get_next(tmp);
					}

					// Connect the node n between the node i-1 and i, becoming the new i -th node
					node_set_next(n, node_get_next(tmp));
					node_set_next(tmp, n);

					// Update head only if the insert was in the head
					if (i == 0) cl->head = n;
				}

				// This means i was 0 and the list is empty (cl->element_count returned 0)
				else {

					// Create the single node in the list
					cl->head = n;
					node_set_next(cl->head, cl->head);
				}
				cl->element_count++;
			}
		}
	}
	return;
}

/**
 * Insert the element pointed to by x as the new head of the list
 */
void cl_insert_head(clinkedlist* cl, void* x) {

	// Check for pointer validity
	if (cl && x) {

		cl_insert_at(cl, x, 0);
	}
	return;
}

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void cl_insert_tail(clinkedlist* cl, void* x) {

	// Check for pointer validity
	if (cl && x) {

		cl_insert_at(cl, x, cl->element_count);
	}
	return;
}

/**
 * Removes the i -th element from the list
 */
void cl_remove_at(clinkedlist* cl, size_t i) {

	// Check for pointer validity
	if (cl) {

		// Check if the position is correct
		if (i < cl->element_count) {
			
			/* Since we need to iterate to the(i - 1) - th element, the one before the first is the last
			 * The cases insertTail and insertHead are the same, however, only when insearting a new head we need
			 * to update the head pointer
			 */

			node* tmp = cl->head;
			node* to_be_deleted = NULL;
			i = (i == 0) ? cl->element_count : i;

			// Iterate to the element before i
			for (int j = 1; j < i; j++) {
				tmp = node_get_next(tmp);
			}

			// Node that will be removed
			to_be_deleted = node_get_next(tmp);

			// If there are more elements in the list
			if (cl->element_count > 1) {

				// Connect the node n between the node i-1 and i+1, logically removing i
				node_set_next(tmp, node_get_next(node_get_next(tmp)));

				// Update head only if we removed the head
				if (i == cl->element_count) cl->head = node_get_next(tmp);
			}
			// If that was the last element in the list
			else {

				// Set the head to null
				cl->head = NULL;
			}
			node_delete(&to_be_deleted);
			cl->element_count--;
		}
	}
	return;
}

/**
 * Removes the head from the list
 */
void cl_remove_head(clinkedlist* cl) {

	// Check for pointer validity
	if (cl) {

		cl_remove_at(cl, 0);
	}
	return;
}

/**
 * Removes the tail from the list
 */
void cl_remove_tail(clinkedlist* cl) {

	// Check for pointer validity
	if (cl) {

		cl_remove_at(cl, cl->element_count - 1);
	}
	return;
}

/**
 * Returns a pointer to the i -th element of the list
 */
void* cl_get_at(clinkedlist* cl, size_t i) {

	void* ret = NULL;

	// Check for pointer validity
	if (cl) {

		// Check if the position is correct
		if (i < cl->element_count) {

			// Iterate to the i -th node
			node* tmp = cl->head;

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
void cl_get_2_at(clinkedlist* cl, size_t i, void* buf) {

	// Check for pointer validity
	if (cl) {

		// Check if the position is correct
		if (i < cl->element_count) {

			// Iterate to the i -th node
			node* tmp = cl->head;

			for (int j = 0; j < i; j++) {

				tmp = node_get_next(tmp);
			}

			// Copy the value
			memcpy(buf, node_get_value(tmp), cl->element_size);
		}
	}
	return;
}

/**
 * Returns a pointer to element stored in the the head of the list
 */
void* cl_get_head(clinkedlist* cl) {

	return cl ? cl_get_at(cl, 0) : NULL;
}

/**
 * Copies the element stored in the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void cl_get_2_head(clinkedlist* cl, void* buf) {

	cl_get_2_at(cl, 0, buf);
	return;
}

/**
 * Returns a pointer to the element stored in the tail of the list
 */
void* cl_get_tail(clinkedlist* cl) {

	return cl ? cl_get_at(cl, cl->element_count - 1) : NULL;
}

/**
 * Copies the element stored in the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void cl_get_2_tail(clinkedlist* cl, void* buf) {

	cl_get_2_at(cl, cl->element_count - 1, buf);
	return;
}

/**
 * Returns the number of elements of the list
 */
size_t cl_get_size(clinkedlist* cl) {

	return cl ? cl->element_count : 0;
}

/**
 * Returns the size of the elements of the list
 */
size_t cl_get_element_size(clinkedlist* cl) {

	return cl ? cl->element_size : 0;
}

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to cl_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short cl_contains(clinkedlist* cl, void* x) {

	int isPresent = 0, index = 0;

	// Check for pointer validity
	if (cl && x) {

		// Iterate through the list and compare with each element
		node* tmp = cl->head;
		for (index = 0; index < cl->element_count; index++) {

			// Compare the i -th element with the element pointed to by x
			isPresent = (memcmp(x, node_get_value(tmp), cl->element_size) == 0);
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
bool cl_is_empty(clinkedlist* cl) {

	return cl ? !(cl->element_count) : true;
}

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void cl_clear(clinkedlist* cl) {

	// Access the list if the pointer is valid
	if (cl) {

		node* to_be_deleted = NULL;
		node* tmp = cl->head;
		for (int i = 0; i < cl->element_count; i++) {

			to_be_deleted = tmp;
			tmp = node_get_next(tmp);
			node_delete(&to_be_deleted);
		}
		cl->head = NULL;
		cl->element_count = 0;
	}
}

/**
 * Applies the function f to every element
 * of the circular linked list cl
 */
void cl_for_each(clinkedlist* cl, void (*f)(void*)) {

	// Parameters check
	if (cl && f) {

		// Iterate the list, applying the function to each element
		node* tmp = cl->head;
		void* tmp_buf = malloc(cl->element_size);
		if (tmp_buf) {

			// Apply the function to each element
			for (int i = 0; i < cl->element_count; i++) {

				// Get a copy of the element for safety reasons
				memcpy(tmp_buf, node_get_value(tmp), cl->element_size);

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
 * Returns a circular linked list obtained by applying
 * the function f to every element of the original list cl
 */
clinkedlist* cl_map(clinkedlist* cl, void* (*f)(void*)) {

	clinkedlist* mapped = NULL;

	// Parameters check
	if (cl && f) {

		mapped = cl_create(cl->element_size);
		if (mapped) {

			// Iterate the list
			node* tmp = cl->head;
			void* tmp_buf = malloc(cl->element_size);
			if (tmp_buf) {

				// Apply the function to each element and insert it in the new list
				for (int i = 0; i < cl->element_count; i++) {

					// Get a copy of the element for safety reasons
					memcpy(tmp_buf, node_get_value(tmp), cl->element_size);

					// Apply the function to the copy and insert it in the list
					cl_insert_at(mapped, f(tmp_buf), i);
					tmp = node_get_next(tmp);
				}
				free(tmp_buf);
			}
		}
	}
	return mapped;
}
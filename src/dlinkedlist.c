/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/dlinkedlist.h"
#include "../include/dnode.h"
#include <string.h>

 /**
  * Struct that represent a list of elements of a generic type value
  */
typedef struct dlinkedlist {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	 /* Pointer to the first node (head) of the list */
	dnode* head;

	/* Number of elements present in the list */
	size_t element_count;

	/* Size of the elements stored in the list */
	size_t element_size;

} dlinkedlist;

/**
 *  Creates a doubled linked list ready to store elements that are as big as the given size
 */
dlinkedlist* dll_create(size_t element_size) {

	dlinkedlist* dll = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		dll = (dlinkedlist*)malloc(sizeof(dlinkedlist));

		if (dll) {

			dll->element_size = element_size;
			dll->element_count = 0;
		}
	}
	return dll;
}

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void dll_delete(dlinkedlist** dll) {

	// Access the list only if the pointer is valid
	if (dll && *dll) {

		// Free every node
		dll_clear(*dll);

		// Free the memory used for the whole struct
		memset(*dll, 0, sizeof(dlinkedlist));
		free(*dll);
		*dll = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void dll_insert_at(dlinkedlist* dll, void* x, size_t i) {

	// Check for pointer validity
	if (dll && x) {

		// Check if the position is correct
		if (i <= dll->element_count) {

			// Create the node
			dnode* dn = dnode_create(x, dll->element_size);

			// Continue only if the node was created
			if (dn) {

				// Since we need to iterate to the (i-1) -th element, the case i = 0 has to be handled differently
				if (i == 0) {

					dnode_set_next(dn, dll->head); // Connect the new node with the old head
					dnode_set_prev(dll->head, dn);
					dll->head = dn; // And replace the list's head
				}

				// In every other case
				else {

					dnode* tmp = dll->head;

					// Iterate to the element before i
					for (int j = 1; j < i; j++) {

						tmp = dnode_get_next(tmp);
					}

					// Connect the node n between the node i-1 and i, becoming the new i -th node
					dnode_set_next(dn, dnode_get_next(tmp));
					if (i < dll->element_count) dnode_set_prev(dnode_get_next(tmp), dn);

					dnode_set_next(tmp, dn);
					dnode_set_prev(dn, tmp);
				}

				dll->element_count++;
			}
		}
	}
	return;
}

/**
 * Insert the element pointed to by x as the new head of the list
 */
void dll_insert_head(dlinkedlist* dll, void* x) {

	// Check for pointer validity
	if (dll && x) {

		dll_insert_at(dll, x, 0);
	}
	return;
}

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void dll_insert_tail(dlinkedlist* dll, void* x) {

	// Check for pointer validity
	if (dll && x) {

		dll_insert_at(dll, x, dll->element_count);
	}
	return;
}

/**
 * Removes the i -th element from the list
 */
void dll_remove_at(dlinkedlist* dll, size_t i) {

	// Check for pointer validity
	if (dll) {

		// Check if the position is correct
		if (i < dll->element_count) {

			// Iterate until the i -th element
			dnode* to_be_deleted = dll->head;
			for (int j = 0; j < i; j++) {

				to_be_deleted = dnode_get_next(to_be_deleted);
			}

			/* We are now at the position i, we remove this node by connecting the one before with the one after
			 *
			 * First we connect the node i-1 with the node i+1 to logically delete the node i
			 * Then, we physically free the memory used for it
			 */
			if (i > 0) dnode_set_next(dnode_get_prev(to_be_deleted), dnode_get_next(to_be_deleted)); // Only if not removing the head
			else dll->head = dnode_get_next(to_be_deleted); // Update head if removed
			if (i < dll->element_count - 1) dnode_set_prev(dnode_get_next(to_be_deleted), dnode_get_prev(to_be_deleted)); // Only if not removing the tail

			dnode_delete(&to_be_deleted);
			dll->element_count--;
		}
	}
	return;
}

/**
 * Removes the head from the list
 */
void dll_remove_head(dlinkedlist* dll) {

	// Check for pointer validity
	if (dll) {

		dll_remove_at(dll, 0);
	}
	return;
}

/**
 * Removes the tail from the list
 */
void dll_remove_tail(dlinkedlist* dll) {

	// Check for pointer validity
	if (dll) {

		dll_remove_at(dll, dll->element_count - 1);
	}
	return;
}

/**
 * Returns a pointer to the i -th element of the list
 */
void* dll_get_at(dlinkedlist* dll, size_t i) {

	void* ret = NULL;

	// Check for pointer validity
	if (dll) {

		// Check if the position is correct
		if (i < dll->element_count) {

			// Iterate to the i -th node
			dnode* tmp = dll->head;

			for (int j = 0; j < i; j++) {

				tmp = dnode_get_next(tmp);
			}

			// Return the value
			ret = dnode_get_value(tmp);
		}
	}
	return ret;
}

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dll_get_2_at(dlinkedlist* dll, size_t i, void* buf) {

	// Check for pointer validity
	if (dll) {

		// Check if the position is correct
		if (i < dll->element_count) {

			// Iterate to the i -th node
			dnode* tmp = dll->head;

			for (int j = 0; j < i; j++) {

				tmp = dnode_get_next(tmp);
			}

			// Copy the value
			memcpy(buf, dnode_get_value(tmp), dll->element_size);
		}
	}
	return;
}

/**
 * Returns a pointer to element stored in the the head of the list
 */
void* dll_get_head(dlinkedlist* dll) {

	return dll ? dll_get_at(dll, 0) : NULL;
}

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dll_get_2_head(dlinkedlist* dll, void* buf) {

	dll_get_2_at(dll, 0, buf);
	return;
}

/**
 * Returns a pointer to element stored in the the tail of the list
 */
void* dll_get_tail(dlinkedlist* dll) {

	return dll ? dll_get_at(dll, dll->element_count - 1) : NULL;
}

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dll_get_2_tail(dlinkedlist* dll, void* buf) {

	dll_get_2_at(dll, dll->element_count - 1, buf);
	return;
}

/**
 * Returns the number of elements of the list
 */
size_t dll_get_size(dlinkedlist* dll) {

	return dll ? dll->element_count : 0;
}

/**
 * Returns the size of the elements of the list
 */
size_t dll_get_element_size(dlinkedlist* dll) {

	return dll ? dll->element_size : 0;
}

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to dll_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short dll_contains(dlinkedlist* dll, void* x) {

	int isPresent = 0, index = 0;

	// Check for pointer validity
	if (dll && x) {

		// Iterate through the list and compare with each element
		dnode* tmp = dll->head;
		for (index = 0; index < dll->element_count; index++) {

			// Compare the i -th element with the element pointed to by x
			isPresent = (memcmp(x, dnode_get_value(tmp), dll->element_size) == 0);
			if (isPresent)
				break;
			tmp = dnode_get_next(tmp);
		}
	}
	return isPresent ? index + 1 : 0;
}

/**
 * Checks whether the list contains at least one element or not
 */
bool dll_is_empty(dlinkedlist* dll) {

	return dll ? !(dll->element_count) : true;
}

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void dll_clear(dlinkedlist* dll) {

	// Access the list if the pointer is valid
	if (dll) {

		dnode* to_be_deleted = NULL;
		dnode* tmp = dll->head;
		for (int i = 0; i < dll->element_count; i++) {

			to_be_deleted = tmp;
			tmp = dnode_get_next(tmp);
			dnode_delete(&to_be_deleted);
		}
		dll->head = NULL;
		dll->element_count = 0;
	}
}

/**
 * Applies the function f to every element
 * of the doubled linked list dll
 */
void dll_for_each(dlinkedlist* dll, void (*f)(void*)) {

	// Parameters check
	if (dll && f) {

		// Iterate the list, applying the function to each element
		dnode* tmp = dll->head;
		void* tmp_buf = malloc(dll->element_size);
		if (tmp_buf) {

			// Apply the function to each element
			for (int i = 0; i < dll->element_count; i++) {

				// Get a copy of the element for safety reasons
				memcpy(tmp_buf, dnode_get_value(tmp), dll->element_size);

				// Apply the function to the copy
				f(tmp_buf);
				tmp = dnode_get_next(tmp);
			}
			free(tmp_buf);
		}
	}
	return;
}

/**
 * Returns a doubled linked list obtained by applying
 * the function f to every element of the original list dll
 */
dlinkedlist* dll_map(dlinkedlist* dll, void* (*f)(void*)){
	
	dlinkedlist* mapped = NULL;

	// Parameters check
	if (dll && f) {

		mapped = dll_create(dll->element_size);
		if (mapped) {

			// Iterate the list
			dnode* tmp = dll->head;
			void* tmp_buf = malloc(dll->element_size);
			if (tmp_buf) {

				// Apply the function to each element and insert it in the new list
				for (int i = 0; i < dll->element_count; i++) {

					// Get a copy of the element for safety reasons
					memcpy(tmp_buf, dnode_get_value(tmp), dll->element_size);

					// Apply the function to the copy and insert it in the list
					dll_insert_at(mapped, f(tmp_buf), i);
					tmp = dnode_get_next(tmp);
				}
				free(tmp_buf);
			}
		}
	}
	return mapped;
}
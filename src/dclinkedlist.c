/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/dclinkedlist.h"
#include "../include/dnode.h"
#include <string.h>

 /**
  * Struct that represent a double linked circular list of elements of a generic type value
  */
typedef struct dclinkedlist {

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

} dclinkedlist;

/**
 *  Creates a double circular linked list ready to store elements that are as big as the given size
 */
dclinkedlist* dcl_create(size_t element_size) {

	dclinkedlist* dcl = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		dcl = (dclinkedlist*)malloc(sizeof(dclinkedlist));

		if (dcl) {

			dcl->element_size = element_size;
			dcl->element_count = 0;
		}
	}
	return dcl;
}

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void dcl_delete(dclinkedlist** dcl) {

	// Access the list only if the pointer is valid
	if (dcl && *dcl) {

		// Free every node
		dcl_clear(*dcl);

		// Free the memory used for the whole struct
		memset(*dcl, 0, sizeof(dclinkedlist));
		free(*dcl);
		*dcl = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void dcl_insert_at(dclinkedlist* dcl, void* x, size_t i) {

	// Check for pointer validity
	if (dcl && x) {

		//Adjust index, being a circular list, after the last element there is the first one
		i = i % (dcl->element_count + 1);

		// Create the node
		dnode* dn = dnode_create(x, dcl->element_size);

		// Continue only if the node was created
		if (dn) {

			size_t actual_index = i % dcl->element_count;

			/* Iterate to the i -th element
			 * Connect the new node with the previous node (i-1)
			 * Connect the new node with the current i -th (will become i+1)
			 */
			dnode* tmp = dcl->head;
			for (int j = 0; j < actual_index; j++) {

				tmp = dnode_get_next(tmp);
			}

			/* We are at the i -th position
			 * Connect the new node with the previous node (i-1)
			 * Connect the new node with the current i -th (will become i+1)
			 */
			dnode_set_next(dnode_get_prev(tmp), dn);
			dnode_set_prev(dn, dnode_get_prev(tmp));

			dnode_set_next(dn, tmp);
			dnode_set_prev(tmp, dn);

			// If the adjusted position is 0 we need to update the head
			if (i == 0) dcl->head = dnode_get_prev(dcl->head);

			dcl->element_count++;
		}
	}
	return;
}

/**
 * Insert the element pointed to by x as the new head of the list
 */
void dcl_insert_head(dclinkedlist* dcl, void* x) {

	// Check for pointer validity
	if (dcl && x) {

		dcl_insert_at(dcl, x, 0);
	}
	return;
}

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void dcl_insert_tail(dclinkedlist* dcl, void* x) {

	// Check for pointer validity
	if (dcl && x) {

		dcl_insert_at(dcl, x, dcl->element_count);
	}
	return;
}

/**
 * Removes the i -th element from the list
 */
void dcl_remove_at(dclinkedlist* dcl, size_t i) {

	// Check for pointer validity
	if (dcl) {

		// Circular list, adjust the index
		i = i % dcl->element_count;

		// Iterate until the i -th element
		dnode* to_be_deleted = dcl->head;
		for (int j = 0; j < i; j++) {

			to_be_deleted = dnode_get_next(to_be_deleted);
		}

		/* We are now at the position i, we remove this node by connecting the one before with the one after
		 *
		 * First we connect the node i-1 with the node i+1 to logically delete the node i
		 * Then, we physically free the memory used for it
		 */
		dnode_set_next(dnode_get_prev(to_be_deleted), dnode_get_next(to_be_deleted));
		dnode_set_prev(dnode_get_next(to_be_deleted), dnode_get_prev(to_be_deleted));

		// Head update
		if (i == 0) dnode_get_next(to_be_deleted);

		dnode_delete(&to_be_deleted);
		dcl->element_count--;
	}
	return;
}

/**
 * Removes the head from the list
 */
void dcl_remove_head(dclinkedlist* dcl) {

	// Check for pointer validity
	if (dcl) {

		dcl_remove_at(dcl, 0);
	}
	return;
}

/**
 * Removes the tail from the list
 */
void dcl_remove_tail(dclinkedlist* dcl) {

	// Check for pointer validity
	if (dcl) {

		dcl_remove_at(dcl, dcl->element_count - 1);
	}
	return;
}

/**
 * Returns a pointer to the i -th element of the list
 */
void* dcl_get_at(dclinkedlist* dcl, size_t i) {

	void* ret = NULL;

	// Check for pointer validity
	if (dcl) {

		//Adjust index
		i = i % dcl->element_count;

		// Iterate to the i -th node
		dnode* tmp = dcl->head;

		for (int j = 0; j < i; j++) {

			tmp = dnode_get_next(tmp);
		}

		// Return the value
		ret = dnode_get_value(tmp);
	}
	return ret;
}

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dcl_get_2_at(dclinkedlist* dcl, size_t i, void* buf) {

	// Check for pointer validity
	if (dcl) {

		// Adjust index
		i = i % dcl->element_count;

		// Iterate to the i -th node
		dnode* tmp = dcl->head;

		for (int j = 0; j < i; j++) {

			tmp = dnode_get_next(tmp);
		}

		// Copy the value
		memcpy(buf, dnode_get_value(tmp), dcl->element_size);
	}
	return;
}

/**
 * Returns a pointer to element stored in the the head of the list
 */
void* dcl_get_head(dclinkedlist* dcl) {

	return dcl ? dcl_get_at(dcl, 0) : NULL;
}

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dcl_get_2_head(dclinkedlist* dcl, void* buf) {

	dcl_get_2_at(dcl, 0, buf);
	return;
}

/**
 * Returns a pointer to element stored in the the tail of the list
 */
void* dcl_get_tail(dclinkedlist* dcl) {

	return dcl ? dcl_get_at(dcl, dcl->element_count - 1) : NULL;
}

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dcl_get_2_tail(dclinkedlist* dcl, void* buf) {

	dcl_get_2_at(dcl, dcl->element_count - 1, buf);
	return;
}

/**
 * Returns the number of elements of the list
 */
size_t dcl_get_size(dclinkedlist* dcl) {

	return dcl ? dcl->element_count : 0;
}

/**
 * Returns the size of the elements of the list
 */
size_t dcl_get_element_size(dclinkedlist* dcl) {

	return dcl ? dcl->element_size : 0;
}

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to dcl_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short dcl_contains(dclinkedlist* dcl, void* x) {

	int isPresent = 0, index = 0;

	// Check for pointer validity
	if (dcl && x) {

		// Iterate through the list and compare with each element
		dnode* tmp = dcl->head;
		for (index = 0; index < dcl->element_count; index++) {

			// Compare the i -th element with the element pointed to by x
			isPresent = (memcmp(x, dnode_get_value(tmp), dcl->element_size) == 0);
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
bool dcl_is_empty(dclinkedlist* dcl) {

	return dcl ? !(dcl->element_count) : true;
}

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void dcl_clear(dclinkedlist* dcl) {

	// Access the list if the pointer is valid
	if (dcl) {

		dnode* to_be_deleted = NULL;
		dnode* tmp = dcl->head;
		for (int i = 0; i < dcl->element_count; i++) {

			to_be_deleted = tmp;
			tmp = dnode_get_next(tmp);
			dnode_delete(&to_be_deleted);
		}
		dcl->head = NULL;
		dcl->element_count = 0;
	}
}

/**
 * Applies the function f to every element
 * of the double circular linked list dcl
 */
void dcl_for_each(dclinkedlist* dcl, void (*f)(void*)) {

	// Parameters check
	if (dcl && f) {

		// Iterate the list, applying the function to each element
		dnode* tmp = dcl->head;
		void* tmp_buf = malloc(dcl->element_size);
		if (tmp_buf) {

			// Apply the function to each element
			for (int i = 0; i < dcl->element_count; i++) {

				// Get a copy of the element for safety reasons
				memcpy(tmp_buf, dnode_get_value(tmp), dcl->element_size);

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
 * Returns a double linked list obtained by applying
 * the function f to every element of the original list dcl
 */
dclinkedlist* dcl_map(dclinkedlist* dcl, void* (*f)(void*)) {

	dclinkedlist* mapped = NULL;

	// Parameters check
	if (dcl && f) {

		mapped = dcl_create(dcl->element_size);
		if (mapped) {

			// Iterate the list
			dnode* tmp = dcl->head;
			void* tmp_buf = malloc(dcl->element_size);
			if (tmp_buf) {

				// Apply the function to each element and insert it in the new list
				for (int i = 0; i < dcl->element_count; i++) {

					// Get a copy of the element for safety reasons
					memcpy(tmp_buf, dnode_get_value(tmp), dcl->element_size);

					// Apply the function to the copy and insert it in the list
					dcl_insert_at(mapped, f(tmp_buf), i);
					tmp = dnode_get_next(tmp);
				}
				free(tmp_buf);
			}
		}
	}
	return mapped;
}
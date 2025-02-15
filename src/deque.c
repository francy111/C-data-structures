/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/deque.h"
#include "../include/dclinkedlist.h"
#include <string.h>

 /**
  * Struct that represent a double ended queue that can store
  * a generic type value
  *
  * Double ended queue permits insertions, as well as removals, both at the head and tail
  */
typedef struct deque {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	 /* Pointer to the actual list that implements the deque, the head of the list is the
	  * queue's head (since it's circular and double linked
	  * we can both operate on the tail and head with O(1) )
	  */
	dclinkedlist* head;
} deque;

/**
 *  Creates a double ended queue ready to store elements that are as big as the given size
 */
deque* deque_create(size_t element_size) {

	deque* d = NULL;

	// If the size is reasonable
	if (0 < element_size && element_size <= SIZE_MAX) {

		// Create the struct
		d = (deque*)malloc(sizeof(deque));

		// If it was created, create the actual list (queue)
		if (d) {

			d->head = dcl_create(element_size);

			// If the list was not created, cancel the creation
			if (!d->head) {
				free(d);
				d = NULL;
			}
		}
	}
	return d;
}

/**
 * Deletes the given queue, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual elements is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void deque_delete(deque** d) {

	if (d && *d) {

		// Delete the struct holding the elements, this will delete each element as well
		dcl_delete(&((*d)->head));
		(*d)->head = NULL;

		// Free the memory for the actual struct
		free(*d);
		*d = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x at the start of the queue
 */
void deque_push_front(deque* d, void* x) {

	if (d && x) {

		dcl_insert_head(d->head, x);
	}
	return;
}

/**
 * Insert the element pointed to by x at the end of the queue
 */
void deque_push_back(deque* d, void* x) {

	if (d && x) {

		dcl_insert_tail(d->head, x);
	}
	return;
}

/**
 * Removes the first element of the queue and returns a copy of it
 */
void* deque_pop_front(deque* d) {

	void* val = NULL;

	if (d) {

		val = malloc(dcl_get_element_size(d->head));

		if (val) {

			memcpy(val, dcl_get_head(d->head), dcl_get_element_size(d->head));
			dcl_remove_head(d->head);
		}
	}

	return val;
}

/**
 * Removes the first element of the queue and copies it in the given buffer
 */
void deque_pop_2_front(deque* d, void* buf) {

	if (d && buf) {

		dcl_get_2_head(d->head, buf);
		dcl_remove_head(d->head);
	}
	return;
}

/**
 * Removes the last element of the queue and returns a copy of it
 */
void* deque_pop_back(deque* d) {

	void* val = NULL;

	if (d) {

		val = malloc(dcl_get_element_size(d->head));

		if (val) {

			memcpy(val, dcl_get_tail(d->head), dcl_get_element_size(d->head));
			dcl_remove_tail(d->head);
		}
	}

	return val;
}

/**
 * Removes the last element of the queue and copies it in the given buffer
 */
void deque_pop_2_back(deque* d, void* buf) {

	if (d && buf) {

		dcl_get_2_tail(d->head, buf);
		dcl_remove_tail(d->head);
	}
	return;
}

/**
 * Returns the first element of the queue
 */
void* deque_peek_front(deque* d) {

	return d ? dcl_get_head(d->head) : NULL;
}

/**
 * Copies the first element of the queue in the given buffer
 */
void deque_peek_2_front(deque* d, void* buf) {

	if (d && buf) dcl_get_2_head(d->head, buf);
	return;
}

/**
 * Returns the last element of the queue
 */
void* deque_peek_back(deque* d) {

	return d ? dcl_get_tail(d->head) : NULL;
}

/**
 * Copies the last element of the queue in the given buffer
 */
void deque_peek_2_back(deque* d, void* buf) {

	if (d && buf) dcl_get_2_tail(d->head, buf);
	return;
}

/**
 * Returns the number of elements inside the queue
 */
size_t deque_get_size(deque* d) {

	return d ? dcl_get_size(d->head) : 0;
}

/**
 * Returns the size of each element of the queue
 */
size_t deque_get_element_size(deque* d) {

	return d ? dcl_get_element_size(d->head) : 0;
}

/**
 * Removed every element from the queue
 * (the queue struct itself is not deleted)
 */
void deque_clear(deque* d) {

	if (d) dcl_clear(d->head);
	return;
}

/**
 * Checks whether or not the queue is empty
 */
bool deque_is_empty(deque* d) {

	return d ? dcl_is_empty(d->head) : true;
}
/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/ringbuffer.h"
#include "../include/vector.h"
#include <string.h>

/* Bit 0, sets the behaviour of the enqueue function on a full list
 *
 *   true -> the first element gets overwritten
 *   false -> the operation fails
 */
#define OVERWRITE_POLICY (1 << 0)

/* Bit 1, tells wheter the ring is full or not, since the case (head == tail) both means empty or full ring
 *
 *   true -> the ring is full
 *   false -> there is at least one free position
 */
#define IS_FULL (1 << 1)

 /**
  * Struct that represent a ring buffer that can store
  * a generic type value
  *
  * A ring buffer is a circular fixed array
  */
typedef struct ringbuffer {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Pointer to the struct that actually stores the elements */
	vector* vec;

	/* Index to the first element of the ring */
	size_t head;

	/* Index to the last element of the ring */
	size_t tail;

	/* Flags containing the overwrite policy and information about the fullness ring */
	int flags;

} ringbuffer;

/**
 *  Creates a ring buffer ready to store elements that are as big as the given size
 */
ringbuffer* ring_create(size_t max_capacity, size_t element_size) {

	ringbuffer* r = NULL;

	// Size check
	if (max_capacity > 0 && element_size > 0) {

		// Size check (2)
		if (max_capacity <= SIZE_MAX / element_size) {

			// Allocate the struct
			r = (ringbuffer*)malloc(sizeof(ringbuffer));

			if (r) {

				// Create the vector
				r->vec = vec_create(max_capacity, element_size);

				// The vector was created
				if (r->vec) {

					r->head = 0;
					r->tail = 0;
					r->flags = 0;
				}

				// The vector was not created, cancel the operation
				else {

					free(r);
					r = NULL;
				}
			}
		}
	}
	return r;
}

/**
 * Deletes the given ring buffer, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual elements is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void ring_delete(ringbuffer** r) {

	// Parameter check
	if (r && *r) {

		vec_delete(&(*r)->vec);
		memset(*r, 0, sizeof(ringbuffer));
		free(*r);
		*r = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x at the end of the ring buffer
 *
 * If the given ring is full, either the first element
 * will be overwritten or the insertion will fail, depending
 * on the overwrite policy
 */
void ring_enqueue(ringbuffer* r, void* x) {

	// Parameter check
	if (r && x) {

		// If the list is full
		if (ring_is_full(r)) {

			// If the policy permits overwriting, do that, otherwise, cancel the operation
			if (r->flags & OVERWRITE_POLICY) {

				vec_remove_at(r->vec, r->head);
				r->head = (r->head + 1) % vec_get_size(r->vec);
				vec_insert_at(r->vec, x, r->tail);
				r->tail = (r->tail + 1) % vec_get_size(r->vec);
			}
		}

		// Insert normally
		else {

			vec_insert_at(r->vec, x, r->tail);
			r->tail = (r->tail + 1) % vec_get_size(r->vec);

			if (r->head == r->tail) r->flags |= IS_FULL;
		}
	}
	return;
}

/**
 * Removes an element the first element of the ring and
 * returns a pointer to (a copy of) it
 *
 * If the given ring is empty, NULL is returned
 */
void* ring_deque(ringbuffer* r) {

	void* val = NULL;

	if (r) {

		if (!ring_is_empty(r)) {

			val = malloc(vec_get_element_size(r->vec));

			if (val) {

				memcpy(val, vec_get_at(r->vec, r->head), vec_get_element_size(r->vec));
				vec_remove_at(r->vec, r->head);
				r->head = (r->head + 1) % vec_get_size(r->vec);
			}
		}
	}
	return val;
}

/**
 * Removes an element the first element of the ring and
 * copies it in the given buffer
 *
 * If the given ring is empty, NULL is returned
 */
void ring_deque_2(ringbuffer* r, void* buf) {

	if (r) {

		if (!ring_is_empty(r)) {

			vec_get_2_at(r->vec, r->head, buf);
			vec_remove_at(r->vec, r->head);
			r->head = (r->head + 1) % vec_get_size(r->vec);
		}
	}
	return;
}

/**
 * Returns the first element of the ring without removing it
 *
 * If the given ring is empty, NULL is returned
 */
void* ring_peek(ringbuffer* r) {

	return r ? vec_get_at(r->vec, r->head) : NULL;
}

/**
 * Copies the first element of the ring in the given buffer, without removing it
 *
 * If the given ring is empty, NULL is returned
 */
void ring_peek_2(ringbuffer* r, void* buf) {

	if (r) vec_get_2_at(r->vec, r->head, buf);
	return;
}

/**
 * Returns the number of elements inside the ring
 */
size_t ring_get_cur_size(ringbuffer* r) {

	return r ? ((ring_is_full(r)) ? ring_get_max_size(r) : ((r->head <= r->tail) ? (r->tail - r->head) : (ring_get_max_size(r) - r->head + r->tail))) : 0;
}

/**
 * Returns the maximum number of elements that can be stored inside the ring
 */
size_t ring_get_max_size(ringbuffer* r) {

	return r ? vec_get_size(r->vec) : 0;
}

/**
 * Returns the size of each element of the ring
 */
size_t ring_get_element_size(ringbuffer* r) {

	return r ? vec_get_element_size(r->vec) : 0;
}

/**
 * Checks whether or not the ring is empty
 */
bool ring_is_empty(ringbuffer* r) {

	return r ? ((r->head == r->tail) && !(r->flags & IS_FULL)) : false;
}

/**
 * Checks whether or not the ring is full
 */
bool ring_is_full(ringbuffer* r) {

	return r ? (r->flags & IS_FULL) : false;
}

/**
 * Enables the overwrite policy on full rings
 */
void ring_enable_overwrite(ringbuffer* r) {

	if (r) r->flags |= OVERWRITE_POLICY;
	return;
}

/**
 * Disables the overwrite policy on full rings
 */
void ring_disable_overwrite(ringbuffer* r) {

	if(r) r->flags &= ~OVERWRITE_POLICY;
	return;
}

/**
 * Sets the overwrite policy on full rings to the given value
 */
void ring_set_overwrite(ringbuffer* r, bool flag_val) {

	if (r) {

		if (flag_val) {

			ring_enable_overwrite(r);
		}
		else {

			ring_disable_overwrite(r);
		}
	}
	return;
}
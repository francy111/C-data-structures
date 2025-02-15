/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef RINGBUFFER__H
#define RINGBUFFER__H

#include <stdbool.h>

 /**
  * Struct that represent a ring buffer that can store
  * a generic type value
  *
  * A ring buffer is a circular fixed array
  */
typedef struct ringbuffer ringbuffer;

/**
 *  Creates a ring buffer ready to store elements that are as big as the given size
 */
ringbuffer* ring_create(size_t max_capacity, size_t element_size);

/**
 * Deletes the given ring buffer, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual elements is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void ring_delete(ringbuffer** s);

/**
 * Insert the element pointed to by x at the end of the ring buffer
 * 
 * If the given ring is full, either the first element
 * will be overwritten or the insertion will fail, depending
 * on the overwrite policy
 */
void ring_enqueue(ringbuffer* r, void* x);

/**
 * Removes an element the first element of the ring and 
 * returns a pointer to (a copy of) it
 * 
 * If the given ring is empty, NULL is returned
 */
void* ring_deque(ringbuffer* r);

/**
 * Removes an element the first element of the ring and
 * copies it in the given buffer
 *
 * If the given ring is empty, NULL is returned
 */
void ring_deque_2(ringbuffer* r, void* buf);

/**
 * Returns the first element of the ring without removing it
 *
 * If the given ring is empty, NULL is returned
 */
void* ring_peek(ringbuffer* r);

/**
 * Copies the first element of the ring in the given buffer, without removing it
 *
 * If the given ring is empty, NULL is returned
 */
void ring_peek_2(ringbuffer* r, void* buf);

/**
 * Returns the number of elements inside the ring
 */
size_t ring_get_cur_size(ringbuffer* r);

/**
 * Returns the maximum number of elements that can be stored inside the ring
 */
size_t ring_get_max_size(ringbuffer* r);

/**
 * Returns the size of each element of the ring
 */
size_t ring_get_element_size(ringbuffer* r);

/**
 * Checks whether or not the ring is empty
 */
bool ring_is_empty(ringbuffer* r);

/**
 * Checks whether or not the ring is full
 */
bool ring_is_full(ringbuffer* r);

/**
 * Enables the overwrite policy on full rings
 */
void ring_enable_overwrite(ringbuffer* r);

/**
 * Disables the overwrite policy on full rings
 */
void ring_disable_overwrite(ringbuffer* r);

/**
 * Sets the overwrite policy on full rings to the given value
 */
void ring_set_overwrite(ringbuffer* r, bool flag_val);

#endif
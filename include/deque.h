/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef deque__H
#define deque__H

#include <stdbool.h>

 /**
  * Struct that represent a double ended queue that can store
  * a generic type value
  *
  * Double ended queue permits insertions, as well as removals, both at the head and tail
  */
typedef struct deque deque;

/**
 *  Creates a double ended queue ready to store elements that are as big as the given size
 */
deque* deque_create(size_t element_size);

/**
 * Deletes the given queue, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void deque_delete(deque** q);

/**
 * Insert the element pointed to by x at the start of the queue
 */
void deque_push_front(deque* d, void* x);

/**
 * Insert the element pointed to by x at the end of the queue
 */
void deque_push_back(deque* d, void* x);

/**
 * Removes the first element of the queue and returns a copy of it
 */
void* deque_pop_front(deque* d);

/**
 * Removes the first element of the queue and copies it in the given buffer
 */
void deque_pop_2_front(deque* d, void* buf);

/**
 * Removes the last element of the queue and returns a copy of it
 */
void* deque_pop_back(deque* d);

/**
 * Removes the last element of the queue and copies it in the given buffer
 */
void deque_pop_2_back(deque* d, void* buf);

/**
 * Returns the first element of the queue
 */
void* deque_peek_front(deque* d);

/**
 * Copies the first element of the queue in the given buffer
 */
void deque_peek_2_front(deque* d, void* buf);

/**
 * Returns the last element of the queue
 */
void* deque_peek_back(deque* d);

/**
 * Copies the last element of the queue in the given buffer
 */
void deque_peek_2_back(deque* d, void* buf);

/**
 * Returns the number of elements inside the queue
 */
size_t deque_get_size(deque* d);

/**
 * Returns the size of each element of the queue
 */
size_t deque_get_element_size(deque* d);

/**
 * Checks whether or not the queue is empty
 */
bool deque_is_empty(deque* d);

#endif
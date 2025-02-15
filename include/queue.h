/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef QUEUE__H
#define QUEUE__H

#include <stdbool.h>

 /**
  * Struct that represent a queue that can store
  * a generic type value
  *
  * Queue uses a FIFO philosophy, insertions are performed at the tail
  * and removals are performed at the head
  */
typedef struct queue queue;

/**
 *  Creates a queue ready to store elements that are as big as the given size
 */
queue* queue_create(size_t element_size);

/**
 * Deletes the given queue, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual elements is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void queue_delete(queue** q);

/**
 * Insert the element pointed to by x in the queue
 */
void queue_enqueue(queue* q, void* x);

/**
 * Removes an element from the queue and returns a pointer to (a copy of) it
 */
void* queue_dequeue(queue* q);

/**
 * Removes an element from the queue and copies it in the given buffer
 */
void queue_dequeue_2(queue* q, void* buf);

/**
 * Returns the last element from the queue without removing it
 */
void* queue_peek(queue* q);

/**
 * Copies the last element from the queue in the given buffer
 */
void queue_peek_2(queue* q, void* buf);

/**
 * Returns the number of elements inside the queue
 */
size_t queue_get_size(queue* q);

/**
 * Returns the size of each element of the queue
 */
size_t queue_get_element_size(queue* q);

/**
 * Removed every element from the queue
 * (the queue struct itself is not deleted)
 */
void queue_clear(queue* q);

/**
 * Checks whether or not the queue is empty
 */
bool queue_is_empty(queue* q);

#endif
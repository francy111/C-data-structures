/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/queue.h"
#include "../include/clinkedlist.h"
#include <string.h>

 /**
  * Struct that represent a queue that can store
  * a generic type value
  *
  * Queue uses a FIFO philosophy, insertions are performed at the tail
  * and removals are performed at the head
  */
typedef struct queue {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	 /* Pointer to the actual list that implements the queue, the head of the list is the
	  * queue's tail (so we can both operate on the tail and head with O(1) ) 
	  */
	clinkedlist* tail;
} queue;

/**
 *  Creates a queue ready to store elements that are as big as the given size
 */
queue* queue_create(size_t element_size) {

	queue* q = NULL;

	// If the size is reasonable
	if (0 < element_size && element_size <= SIZE_MAX) {

		// Create the struct
		q = (queue*)malloc(sizeof(queue));

		// If it was created, create the actual list (queue)
		if (q) {

			q->tail = cl_create(element_size);

			// If the list was not created, cancel the creation
			if (!q->tail) {
				free(q);
				q = NULL;
			}
		}
	}
	return q;
}

/**
 * Deletes the given queue, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual elements is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void queue_delete(queue** q) {

	if (q && *q) {

		// Delete the struct holding the elements, this will delete each element as well
		cl_delete(&((*q)->tail));
		(*q)->tail = NULL;

		// Free the memory for the actual struct
		free(*q);
		*q = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x in the queue
 */
void queue_enqueue(queue* q, void* x) {

	if (q && x) {

		/* Enqueue means inserting a new last element (new tail)
		 * Our list pointer (list head) is the queue's tail
		 *
		 * We need to add an element AFTER our tail, and since
		 * it's a circular list, a new element between the tail
		 * and the head, this would mean insert a new head (insert at 0)
		 *
		 * However our pointer is to the tail, so we would need to insert
		 * before the next (insert at 1)
		 */
		cl_insert_tail(q->tail, x);
	}
	return;
}

/**
 * Removes an element from the queue and returns a pointer to (a copy of) it
 */
void* queue_dequeue(queue* q) {

	void* val = NULL;

	/* Dequeue means removing the current head
	 * Our list pointer (list head) is the queue's tail
	 *
	 * We need to remove the element AFTER our tail, and since
	 * it's a circular list, just removing the element marked as tail->next
	 *
	 * Since our pointer is to the tail, so we would need to remove the next (remove at 1)
	 */
	if (q) {

		val = malloc(cl_get_element_size(q->tail));

		if (val) {

			memcpy(val, cl_get_head(q->tail), cl_get_element_size(q->tail));
			cl_remove_head(q->tail);
		}
	}
	return val;
}

/**
 * Removes an element from the queue and copies it in the given buffer
 */
void queue_dequeue_2(queue* q, void* buf) {

	if (q && buf) {

		cl_get_2_head(q->tail, buf);
		cl_remove_head(q->tail);
	}
	return;
}

/**
 * Returns the last element from the queue without removing it
 */
void* queue_peek(queue* q) {

	return q ? cl_get_head(q->tail) : NULL;
}

/**
 * Copies the last element from the queue in the given buffer
 */
void queue_peek_2(queue* q, void* buf) {

	if (q && buf) cl_get_2_head(q->tail, buf);
	return;
}

/**
 * Returns the number of elements inside the queue
 */
size_t queue_get_size(queue* q) {

	return q ? cl_get_size(q->tail) : 0;
}

/**
 * Returns the size of each element of the queue
 */
size_t queue_get_element_size(queue* q) {

	return q ? cl_get_element_size(q->tail) : 0;
}

/**
 * Removed every element from the queue
 * (the queue struct itself is not deleted)
 */
void queue_clear(queue* q) {

	if (q) cl_clear(q->tail);
	return;
}

/**
 * Checks whether or not the queue is empty
 */
bool queue_is_empty(queue* q) {

	return q ? cl_is_empty(q->tail) : false;
}
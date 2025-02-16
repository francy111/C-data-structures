/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/stack.h"
#include "../../include/linear/linkedlist.h"
#include <string.h>

 /**
  * Struct that represent a stack that can store
  * a generic type value
  *
  * Stack uses a LIFO philosophy, both insertions and
  * removals are performed at the head of the stack
  */
typedef struct stack {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Pointer to the actual list that implements the stack, the head of the list is the stack's top */
	linkedlist* top;

} stack;

/**
 *  Creates a stack ready to store elements that are as big as the given size
 */
stack* stack_create(size_t element_size) {

	stack* s = NULL;
	
	// If the size is reasonable
	if (0 < element_size && element_size <= SIZE_MAX) {
	
		// Create the struct
		s = (stack*)malloc(sizeof(stack));

		// If it was created, create the actual list (stack)
		if (s) {

			s->top = ll_create(element_size);

			// If the list was not created, cancel the creation
			if (!s->top) {
				free(s);
				s = NULL;
			}
		}
	}
	return s;
}

/**
 * Deletes the given stack, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual elements is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void stack_delete(stack** s) {

	if (s && *s) {

		// Delete the struct holding the elements, this will delete each element as well
		ll_delete(&((*s)->top));
		(*s)->top = NULL;

		// Free the memory for the actual struct
		free(*s);
		*s = NULL;
	}
	return;
}

/**
 * Insert the element pointed to by x in the stack
 */
void stack_push(stack* s, void* x) {

	if (s && x) {

		// Pushing in a stack means inserting at the top
		ll_insert_head(s->top, x);
	}
	return;
}

/**
 * Removes an element from the stack and returns a pointer to (a copy of) it
 */
void* stack_pop(stack* s) {
	
	void* val = NULL;

	if (s) {

		val = malloc(ll_get_element_size(s->top));

		if (val) {
		
			memcpy(val, ll_get_head(s->top), ll_get_element_size(s->top));
			ll_remove_head(s->top);
		}
	}
	return val;
}

/**
 * Removes an element from the stack and copies it in the given buffer
 */
void stack_pop_2(stack* s, void* buf) {

	if (s && buf) {

		ll_get_2_head(s->top, buf);
		ll_remove_head(s->top);
	}

	return;
}

/**
 * Returns the element at the top of the stack without removing it
 */
void* stack_peek(stack* s) {

	return s ? ll_get_head(s->top) : NULL;
}

/**
 * Copies the value at the top of the stack in the given buffer without removing it
 */
void stack_peek_2(stack* s, void* buf) {

	if (s && buf) ll_get_2_head(s->top, buf);
	return;
}

/**
 * Returns the number of elements inside the stack
 */
size_t stack_get_size(stack* s) {

	return s ? ll_get_size(s->top) : 0;
}

/**
 * Returns the size of each element of the stack
 */
size_t stack_get_element_size(stack* s) {

	return s ? ll_get_element_size(s->top) : 0;
}

/**
 * Removed every element from the stack
 * (the stack struct itself is not deleted)
 */
void stack_clear(stack* s) {

	if (s) ll_clear(s->top);
	return;
}

/**
 * Checks whether or not the stack is empty
 */
bool stack_is_empty(stack* s) {

	return s ? ll_is_empty(s->top) : false;
}
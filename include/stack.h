/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef STACK__H
#define STACK__H

#include <stdbool.h>

 /**
  * Struct that represent a stack that can store
  * a generic type value
  *
  * Stack uses a LIFO philosophy, both insertions and 
  * removals are performed at the head of the stack
  */
typedef struct stack stack;

/**
 *  Creates a stack ready to store elements that are as big as the given size
 */
stack* stack_create(size_t element_size);

/**
 * Deletes the given stack, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void stack_delete(stack** s);

/**
 * Insert the element pointed to by x in the stack
 */
void stack_push(stack* s, void* x);

/**
 * Removes an element from the stack and returns a pointer to (a copy of) it
 */
void* stack_pop(stack* s);

/**
 * Removes an element from the stack and copies it in the given buffer
 */
void stack_pop_2(stack* s, void* buf);

/**
 * Returns the element at the top of the stack without removing it
 */
void* stack_peek(stack* s);

/**
 * Copies the value at the top of the stack in the given buffer without removing it
 */
void stack_peek_2(stack* s, void* buf);

/**
 * Returns the number of elements inside the stack
 */
size_t stack_get_size(stack* s);

/**
 * Returns the size of each element of the stack
 */
size_t stack_get_element_size(stack* s);

/**
 * Checks whether or not the stack is empty
 */
bool stack_is_empty(stack* s);

#endif
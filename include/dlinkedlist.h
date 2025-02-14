/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef DLINKEDLIST__H
#define DLINKEDLIST__H

#include <stdlib.h>
#include <stdbool.h>

 /**
  * Struct that represent a list of elements of a generic type value
  */
typedef struct dlinkedlist dlinkedlist;

/**
 *  Creates a doubled linked list ready to store elements that are as big as the given size
 */
dlinkedlist* dll_create(size_t element_size);

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void dll_delete(dlinkedlist** dll);

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void dll_insert_at(dlinkedlist* dll, void* x, size_t i);

/**
 * Insert the element pointed to by x as the new head of the list
 */
void dll_insert_head(dlinkedlist* dll, void* x);

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void dll_insert_tail(dlinkedlist* dll, void* x);

/**
 * Removes the i -th element from the list
 */
void dll_remove_at(dlinkedlist* dll, size_t i);

/**
 * Removes the head from the list
 */
void dll_remove_head(dlinkedlist* dll);

/**
 * Removes the tail from the list
 */
void dll_remove_tail(dlinkedlist* dll);

/**
 * Returns a pointer to the i -th element of the list
 */
void* dll_get_at(dlinkedlist* dll, size_t i);

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dll_get_2_at(dlinkedlist* dll, size_t i, void* buf);

/**
 * Returns a pointer to element stored in the the head of the list
 */
void* dll_get_head(dlinkedlist* dll);

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dll_get_2_head(dlinkedlist* dll, void* buf);

/**
 * Returns a pointer to element stored in the the tail of the list
 */
void* dll_get_tail(dlinkedlist* dll);

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dll_get_2_tail(dlinkedlist* dll, void* buf);

/**
 * Returns the number of elements of the list
 */
size_t dll_get_size(dlinkedlist* dll);

/**
 * Returns the size of the elements of the list
 */
size_t dll_get_element_size(dlinkedlist* dll);

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to dll_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short dll_contains(dlinkedlist* dll, void* x);

/**
 * Checks whether the list contains at least one element or not
 */
bool dll_is_empty(dlinkedlist* dll);

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void dll_clear(dlinkedlist* dll);

/**
 * Applies the function f to every element
 * of the doubled linked list dll
 */
void dll_for_each(dlinkedlist* dll, void (*f)(void*));

/**
 * Returns a doubled linked list obtained by applying
 * the function f to every element of the original list dll
 */
dlinkedlist* dll_map(dlinkedlist* dll, void* (*f)(void*));

#endif
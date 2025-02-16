/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef DCLINKEDLIST__H
#define DCLINKEDLIST__H

#include <stdlib.h>
#include <stdbool.h>

 /**
  * Struct that represent a double linked circular list of elements of a generic type value
  */
typedef struct dclinkedlist dclinkedlist;

/**
 *  Creates a double circular linked list ready to store elements that are as big as the given size
 */
dclinkedlist* dcl_create(size_t element_size);

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void dcl_delete(dclinkedlist** dcl);

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void dcl_insert_at(dclinkedlist* dcl, void* x, size_t i);

/**
 * Insert the element pointed to by x as the new head of the list
 */
void dcl_insert_head(dclinkedlist* dcl, void* x);

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void dcl_insert_tail(dclinkedlist* dcl, void* x);

/**
 * Removes the i -th element from the list
 */
void dcl_remove_at(dclinkedlist* dcl, size_t i);

/**
 * Removes the head from the list
 */
void dcl_remove_head(dclinkedlist* dcl);

/**
 * Removes the tail from the list
 */
void dcl_remove_tail(dclinkedlist* dcl);

/**
 * Returns a pointer to the i -th element of the list
 */
void* dcl_get_at(dclinkedlist* dcl, size_t i);

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dcl_get_2_at(dclinkedlist* dcl, size_t i, void* buf);

/**
 * Returns a pointer to element stored in the the head of the list
 */
void* dcl_get_head(dclinkedlist* dcl);

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dcl_get_2_head(dclinkedlist* dcl, void* buf);

/**
 * Returns a pointer to element stored in the the tail of the list
 */
void* dcl_get_tail(dclinkedlist* dcl);

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void dcl_get_2_tail(dclinkedlist* dcl, void* buf);

/**
 * Returns the number of elements of the list
 */
size_t dcl_get_size(dclinkedlist* dcl);

/**
 * Returns the size of the elements of the list
 */
size_t dcl_get_element_size(dclinkedlist* dcl);

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to dcl_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short dcl_contains(dclinkedlist* dcl, void* x);

/**
 * Checks whether the list contains at least one element or not
 */
bool dcl_is_empty(dclinkedlist* dcl);

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void dcl_clear(dclinkedlist* dcl);

/**
 * Applies the function f to every element
 * of the double circular linked list dcl
 */
void dcl_for_each(dclinkedlist* dcl, void (*f)(void*));

/**
 * Returns a double circular linked list obtained by applying
 * the function f to every element of the original list dcl
 */
dclinkedlist* dcl_map(dclinkedlist* dcl, void* (*f)(void*));

#endif
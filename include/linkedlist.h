/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef LINKEDLIST__H
#define LINKEDLIST__H

#include <stdlib.h>
#include <stdbool.h>

 /**
  * Struct that represent a list of elements of a generic type value
  */
typedef struct linkedlist linkedlist;

/**
 *  Creates a linked list ready to store elements that are as big as the given size
 */
linkedlist* ll_create(size_t element_size);

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void ll_delete(linkedlist** ll);

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void ll_insert_at(linkedlist* ll, void* x, size_t i);

/**
 * Insert the element pointed to by x as the new head of the list
 */
void ll_insert_head(linkedlist* ll, void* x);

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void ll_insert_tail(linkedlist* ll, void* x);

/**
 * Removes the i -th element from the list
 */
void ll_remove_at(linkedlist* ll, size_t i);

/**
 * Removes the head from the list
 */
void ll_remove_head(linkedlist* ll);

/**
 * Removes the tail from the list
 */
void ll_remove_tail(linkedlist* ll);

/**
 * Returns a pointer to the i -th element of the list
 */
void* ll_get_at(linkedlist* ll, size_t i);

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_at(linkedlist* ll, size_t i, void* buf);

/**
 * Returns a pointer to the head of the list
 */
void* ll_get_head(linkedlist* ll);

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_head(linkedlist* ll, void* buf);

/**
 * Returns a pointer to the tail of the list
 */
void* ll_get_tail(linkedlist* ll);

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void ll_get_2_tail(linkedlist* ll, void* buf);

/**
 * Returns the number of elements of the list
 */
size_t ll_get_size(linkedlist* ll);

/**
 * Returns the size of the elements of the list
 */
size_t ll_get_element_size(linkedlist* ll);

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to ll_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short ll_contains(linkedlist* ll, void* x);

/**
 * Checks whether the list contains at least one element or not
 */
bool ll_is_empty(linkedlist* ll);

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void ll_clear(linkedlist* ll);

#endif
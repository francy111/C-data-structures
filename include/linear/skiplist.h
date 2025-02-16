/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef SKIPLIST__H
#define SKIPLIST__H

#include <stdlib.h>
#include <stdbool.h>

 /**
  * Struct that represent a list of elements of a generic type value
  */
typedef struct skiplist skiplist;

/**
 *  Creates a linked list ready to store elements that are as big as the given size
 */
skiplist* sl_create(size_t element_size, size_t max_levels, double probability, int (*cmp)(void*, void*));

/**
 * Deletes the given list
 */
void sl_delete(skiplist** sl);

/**
 * Insert the element pointed to by x in the skiplist
 */
void sl_insert(skiplist* sl, void* x);

/**
 * Removes x from the list (if present)
 */
void sl_remove(skiplist* sl, void* x);

/**
 * Returns a pointer to the node containing x
 */
void* sl_search(skiplist* sl, void* x);

/**
 * Returns the number of elements of the list
 */
size_t sl_get_size(skiplist* sl);

/**
 * Returns the size of the elements of the list
 */
size_t sl_get_element_size(skiplist* sl);

/**
 * Returns the number of levels in the skip list
 */
size_t sl_get_max_levels(skiplist* sl);

/**
 * Checks if the element pointed to by x is present in the list
 */
bool sl_contains(skiplist* sl, void* x);

/**
 * Checks whether the list contains at least one element or not
 */
bool sl_is_empty(skiplist* sl);

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void sl_clear(skiplist* sl);

#endif
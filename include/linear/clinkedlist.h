/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef CLINKEDLIST__H
#define CLINKEDLIST__H

#include <stdlib.h>
#include <stdbool.h>

 /**
  * Struct that represent a circular list of elements of a generic type value
  */
typedef struct clinkedlist clinkedlist;

/**
 *  Creates a circular linked list ready to store elements that are as big as the given size
 */
clinkedlist* cl_create(size_t element_size);

/**
 * Deletes the given list, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual element in each node, and the node, are freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct is then set to NULL
 */
void cl_delete(clinkedlist** cl);

/**
 * Insert the element pointed to by x as the i -th element of the list
 */
void cl_insert_at(clinkedlist* cl, void* x, size_t i);

/**
 * Insert the element pointed to by x as the new head of the list
 */
void cl_insert_head(clinkedlist* cl, void* x);

/**
 * Insert the element pointed to by x as the new tail of the list
 */
void cl_insert_tail(clinkedlist* cl, void* x);

/**
 * Removes the i -th element from the list
 */
void cl_remove_at(clinkedlist* cl, size_t i);

/**
 * Removes the head from the list
 */
void cl_remove_head(clinkedlist* cl);

/**
 * Removes the tail from the list
 */
void cl_remove_tail(clinkedlist* cl);

/**
 * Returns a pointer to the i -th element of the list
 */
void* cl_get_at(clinkedlist* cl, size_t i);

/**
 * Copies the i -th element of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void cl_get_2_at(clinkedlist* cl, size_t i, void* buf);

/**
 * Returns a pointer to element stored in the the head of the list
 */
void* cl_get_head(clinkedlist* cl);

/**
 * Copies the head of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void cl_get_2_head(clinkedlist* cl, void* buf);

/**
 * Returns a pointer to element stored in the the tail of the list
 */
void* cl_get_tail(clinkedlist* cl);

/**
 * Copies the tail of the list inside the buffer
 * pointed to by buf (we assume it has already been allocated,
 * and of the correct size)
 */
void cl_get_2_tail(clinkedlist* cl, void* buf);

/**
 * Returns the number of elements of the list
 */
size_t cl_get_size(clinkedlist* cl);

/**
 * Returns the size of the elements of the list
 */
size_t cl_get_element_size(clinkedlist* cl);

/**
 * Checks if the element pointed to by x is present in the list
 *
 * The value returned is actually it's position in the list
 * from 1 to cl_get_size (needs to be adjusted by subtracting one when accessing the list)
 */
short cl_contains(clinkedlist* cl, void* x);

/**
 * Checks whether the list contains at least one element or not
 */
bool cl_is_empty(clinkedlist* cl);

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void cl_clear(clinkedlist* cl);

/**
 * Applies the function f to every element
 * of the circular linked list cl
 */
void cl_for_each(clinkedlist* cl, void (*f)(void*));

/**
 * Returns a circular linked list obtained by applying
 * the function f to every element of the original list cl
 */
clinkedlist* cl_map(clinkedlist* cl, void* (*f)(void*));

#endif
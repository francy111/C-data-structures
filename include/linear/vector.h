/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef VECTOR__H
#define VECTOR__H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Struct that represent a generic type vector
 *
 * Elements can be of any type
 */
typedef struct vector vector;

/**
 *  Creates a vector with the given size, ready to store elements that are 'element_size' long
 */
vector* vec_create(size_t vector_size, size_t element_size);

/**
 * Deletes the given vector, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual array of data is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct (*v) is set to NULL
 * 
 * N.B
 * If the added elements were passed as struct to pointers, each element of
 * the array would contain a copy of the struct, after deletion that memory would be free
 * However, if it was passed as a pointer to a pointer to the struct, then each 
 * array element would store the pointer to the struct, after deletion, the memory for
 * said structs would still be allocated
 */
void vec_delete(vector** v);

/**
 * Performs the action v[i] = x
 * 
 * To be more specific, it copies the value pointed to by x into the
 * i -th position of the vector. We assume the pointed value is of the size
 * given when creating the vector
 */
void vec_insert_at(vector* v, void* x, size_t i);

/**
 * Removes the i -th element from the vector
 * In other words, Performs the action v[i] = del.value
 *
 * To be more specific, it sets the whole i -th element 
 * (all the bytes of such element, based on element size) to be zero (0)
 */
void vec_remove_at(vector* v, size_t i);

/**
 * Returns a pointer to the i -th element of v, so v(i)
 * 
 * To be more specific, it returns the memory address
 * of the first byte of the i -th element. This is returned
 * as a void pointer, which should be casted by the caller
 */
void* vec_get_at(vector* v, size_t i);

/**
 * Copies the i -th element of the array, v(i) inside the buffer
 * pointed to by buf
 */
void vec_get_2_at(vector* v, size_t i, void* buf);

/**
 * Returns the vector size of v
 */
size_t vec_get_size(vector* v);

/**
 * Returns the element size of v
 */
size_t vec_get_element_size(vector* v);

/**
 * Checks if the element pointed to by x is present in the vector
 *
 * The value returned is actually it's position in the array
 * from 1 to vec_get_size (needs to be adjusted by subtracting one when accessing the vector)
 */
short vec_contains(vector* v, void* x);

/**
 * Checks if the i -th element of the vector is empty or not
 *
 * With empty we mean that each byte is set to 0
 */
short vec_is_index_empty(vector* v, size_t i);

/**
 * Empties every element, but
 * doesn't deallocate the memory
 */
void vec_clear(vector* v);

/**
 * Applies the function f to every element
 * of the vector v
 */
void vec_for_each(vector* v, void (*f)(void*));

/**
 * Returns a vector obtained by applying 
 * the function f to every element of the vector v
 */
vector* vec_map(vector* v, void* (*f)(void*));

#endif

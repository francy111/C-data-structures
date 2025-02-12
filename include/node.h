/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#ifndef NODE__H
#define NODE__H

#include <stdlib.h>

/**
 * Struct that represent a node that can store
 * a generic type value
 *
 * Used in Linked Lists and Circular Linked List
 * 
 * The node itself has two components, a value stored, and
 * a pointer to the next node in the list
 */
typedef struct node node;

/**
 * Creates a node that will store the content pointed to by 'value'
 * That will be written in 'value_size' bytes
 * 
 * eg. to store an int, value_size = sizeof(int)
 */
node* node_create(void* value, size_t value_size);

/**
 * Deletes the given node
 * 
 * It frees the memory used to store the value, then
 * the memory used for the struct itself
 */
void node_delete(node** n);

/**
 * Returns a pointer to the value stored in the
 * node
 *
 * Useful for returning structs, without
 * the need to copy all of its content
 */
void* node_get_value(node* n);

/**
 * Returns a pointer to the next node, that is
 * the node pointed to by this one
 */
node* node_get_next(node* n);

/**
 * Sets 'next' as the next node, that is the
 * node pointed to by this one
 */
void node_set_next(node* n, node* next);

#endif
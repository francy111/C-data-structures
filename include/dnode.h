/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef DNODE__H
#define DNODE__H

#include <stdlib.h>

/**
 * Struct that represent a node that can store
 * a generic type value, and has pointers
 * to both the previous and next element
 *
 * Used in Double Linked Lists and Double Circular Linked Lists
 * 
 * The node itself has three components, a value stored,
 * a pointer to the next node in the list, and a pointer
 * to the previous node
 */
typedef struct dnode dnode;

/**
 * Creates a node that will store the content pointed to by 'value'
 * That will be written in 'value_size' bytes
 * 
 * eg. to store an int, value_size = sizeof(int)
 */
dnode* dnode_create(void* value, size_t value_size);

/**
 * Deletes the given node
 * 
 * It frees the memory used to store the value, then
 * the memory used for the struct itself
 */
void dnode_delete(dnode** dn);

/**
 * Returns a pointer to the value stored in the
 * node
 *
 * Useful for returning structs, without
 * the need to copy all of its content
 */
void* dnode_get_value(dnode* dn);

/**
 * Returns a pointer to the next node
 */
dnode* dnode_get_next(dnode* dn);

/**
 * Sets 'next' as the next node
 */
void dnode_set_next(dnode* dn, dnode* next);

/**
 * Returns a pointer to the previous node
 */
dnode* dnode_get_prev(dnode* dn);

/**
 * Sets 'prev' as the previous node
 */
void dnode_set_prev(dnode* dn, dnode* prev);

#endif
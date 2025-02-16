/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef SNODE__H
#define SNODE__H

#include <stdlib.h>
#include <stdint.h>

/**
 * Struct that represent a node that can store
 * a generic type value
 *
 * Used in Skip Lists
 * 
 * The node itself has two components, a value stored, and
 * an array of pointers to the nodes at the next levels
 */
typedef struct snode snode;

/**
 * Creates a node that will store the content pointed to by 'value'
 * That will be written in 'value_size' bytes
 * 
 * eg. to store an int, value_size = sizeof(int)
 */
snode* snode_create(void* value, size_t value_size, size_t level);

/**
 * Deletes the given node
 * 
 * It frees the memory used to store the value, then
 * the memory used for the struct itself
 */
void snode_delete(snode** sn);

/**
 * Returns a pointer to the value stored in the
 * node
 *
 * Useful for returning structs, without
 * the need to copy all of its content
 */
void* snode_get_value(snode* sn);

/**
 * Returns a pointer to the next node of the given level, that is
 * the node pointed to by this one
 */
snode* snode_get_next(snode* sn, size_t level);

/**
 * Sets 'next' as the next node in the given level, that is the
 * node pointed to by this one
 */
void snode_set_next(snode* sn, snode* snext, size_t level);

/**
 * Returns the level of the node, inside the list it is part of
 */
size_t snode_get_level(snode* sn);

/**
 * Sets the level of the node inside the list it is part of
 */
void snode_set_level(snode* sn, size_t level);

#endif
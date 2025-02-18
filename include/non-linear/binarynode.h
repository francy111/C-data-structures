/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef BINARYNODE__H
#define BINARYNODE__H

#include <stdlib.h>
#include <stdbool.h>

/** 
 * Struct that implements a binary node, that can be used in binary trees
 * 
 * This has a generic type value, a left child (binary node), and a right child (binary node)
 */
typedef struct binarynode binarynode;

/**
 * Creates a binary node that can store elements of the given size
 */
binarynode* binarynode_create(void* x, size_t element_size);

/**
 * Deletes the given binary node
 */
void binarynode_delete(binarynode** bn);

/**
 * Returns the stored value
 */
void* binarynode_get_value(binarynode* bn);

/**
 * Sets the second binary node as father of the first
 */
void binarynode_set_father(binarynode* bn, binarynode* father);

/**
 * Sets the second binary node as left child of the first
 */
void binarynode_set_left_child(binarynode* bn, binarynode* left);

/**
 * Sets the second binary node as right child of the first
 */
void binarynode_set_right_child(binarynode* bn, binarynode* right);

/**
 * Returns the father
 */
binarynode* binarynode_get_father(binarynode* bn);

/**
 * Returns the left child
 */
binarynode* binarynode_get_left_child(binarynode* bn);

/**
 * Returns the right child
 */
binarynode* binarynode_get_right_child(binarynode* bn);

/**
 * Checks if the node is a leaf or not (0 - 1 and 2 children respectively)
 */
bool binarynode_is_leaf(binarynode* bn);

/**
 * Returns the height of the tree whose root is bt
 */
int binarynode_get_height(binarynode* bn);

/**
 * Returns the balance of the tree whose root is bt
 */
int binarynode_get_balance(binarynode* bn);

#endif
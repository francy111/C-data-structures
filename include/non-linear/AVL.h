/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef AVLTREE__H
#define AVLTREE__H

#include <stdlib.h>
#include <stdbool.h>
#include "binarynode.h"

 /**
  * Struct that implements an AVL tree, a struct that is used to store and then search
  * elements, as the structure itself is ordered
  *
  * A BST is a binary tree in which each node holds a value that is, greater than all values
  * stored in the left child's tree, and less than all the values in the right child's tree
  *
  * An AVL is a BST in which each node has a balance (difference between the left subtree's height and the right subtree's height)
  * of either -1, 0 or 1
  * 
  * This has a generic type value, so a compare function needs to be given
  */
typedef struct AVL AVL;

/**
 * Creates a binary node that can store elements of the given size
 */
AVL* AVL_create(size_t element_size, int (*compare)(void*, void*));

/**
 * Deletes the given binary node
 */
void AVL_delete(AVL** avl);

/**
 * Inserts the value x in the AVL, according to the compare function
 */
void AVL_insert(AVL* avl, void* x);

/**
 * Removes the value x from the AVL, if it is present
 */
void AVL_remove(AVL* avl, void* x);

/**
 * Searches the value x in the AVL, returns a pointer to it's node
 */
binarynode* AVL_search(AVL* avl, void* x);

/**
 * Checks wheter or not the value x is present in the tree
 */
bool AVL_contains(AVL* avl, void* x);

/**
 * Returns the minimum value in the tree
 */
void* AVL_min(AVL* avl);

/**
 * Copies the minimum value in the tree inside the buffer
 */
void AVL_min_2(AVL* avl, void* buf);

/**
 * Returns the maximum value in the tree
 */
void* AVL_max(AVL* avl);

/**
 * Copies the maximum value in the tree inside the buffer
 */
void AVL_max_2(AVL* avl, void* buf);

/**
 * Returns the predecessor of the given node inside the tree
 */
binarynode* AVL_predecessor(AVL* avl, binarynode* bn);

/**
 * Returns the successor of the given node inside the tree
 */
binarynode* AVL_successor(AVL* avl, binarynode* bn);

/**
 * Traverses the tree in preorder, applying the function callback to each element
 */
void AVL_traverse_preoder(AVL* avl, void (*callback)(void*));

/**
 * Traverses the tree in inorder, applying the function callback to each element
 */
void AVL_traverse_inoder(AVL* avl, void (*callback)(void*));

/**
 * Traverses the tree in postorder, applying the function callback to each element
 */
void AVL_traverse_postoder(AVL* avl, void (*callback)(void*));

/**
 * Returns the number of elements in the tree
 */
size_t AVL_get_size(AVL* avl);

/**
 * Returns the size of elements stored in the tree
 */
size_t AVL_get_element_size(AVL* avl);

/**
 * Removes each element from the tree, the struct itself is preserved
 */
void AVL_clear(AVL* avl);

/**
 * Checks wheter or not the tree is empty
 */
bool AVL_is_empty(AVL* avl);

/**
 * Returns the compare function used in the tree
 */
void* AVL_get_compare_func(AVL* avl);

/**
 * Returns the height of the tree avl
 */
size_t AVL_get_height(AVL* avl);

#endif
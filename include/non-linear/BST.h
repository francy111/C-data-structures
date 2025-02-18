/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef BINARYSEARCHTREE__H
#define BINARYSEARCHTREE__H

#include <stdlib.h>
#include <stdbool.h>
#include "binarynode.h"

 /**
  * Struct that implements a binary search tree, a struct that is used to store and then search
  * elements, as the structure itself is ordered.
  * 
  * A BST is a binary tree in which each node holds a value that is, greater than all values
  * stored in the left child's tree, and less than all the values in the right child's tree
  *
  * This has a generic type value, so a compare function needs to be given
  */
typedef struct BST BST;

/**
 * Creates a binary node that can store elements of the given size
 */
BST* BST_create(size_t element_size, int (*compare)(void*, void*));

/**
 * Deletes the given binary node
 */
void BST_delete(BST** bst);

/**
 * Inserts the value x in the BST, according to the compare function
 */
void BST_insert(BST* bst, void* x);

/**
 * Removes the value x from the BST, if it is present
 */
void BST_remove(BST* bst, void* x);

/**
 * Searches the value x in the BST, returns a pointer to it's node
 */
binarynode* BST_search(BST* bst, void* x);

/**
 * Checks wheter or not the value x is present in the tree
 */
bool BST_contains(BST* bst, void* x);

/**
 * Returns the minimum value in the tree
 */
void* BST_min(BST* bst);

/**
 * Copies the minimum value in the tree inside the buffer
 */
void BST_min_2(BST* bst, void* buf);

/**
 * Returns the maximum value in the tree
 */
void* BST_max(BST* bst);

/**
 * Copies the maximum value in the tree inside the buffer
 */
void BST_max_2(BST* bst, void* buf);

/**
 * Returns the predecessor of the given node inside the tree
 */
binarynode* BST_predecessor(BST* bst, binarynode* bn);

/**
 * Returns the successor of the given node inside the tree
 */
binarynode* BST_successor(BST* bst, binarynode* bn);

/**
 * Traverses the tree in preorder, applying the function callback to each element
 */
void BST_traverse_preoder(BST* bst, void (*callback)(void*));

/**
 * Traverses the tree in inorder, applying the function callback to each element
 */
void BST_traverse_inoder(BST* bst, void (*callback)(void*));

/**
 * Traverses the tree in postorder, applying the function callback to each element
 */
void BST_traverse_postoder(BST* bst, void (*callback)(void*));

/**
 * Returns the number of elements in the tree
 */
size_t BST_get_size(BST* bst);

/**
 * Returns the size of elements stored in the tree
 */
size_t BST_get_element_size(BST* bst);

/**
 * Removes each element from the tree, the struct itself is preserved
 */
void BST_clear(BST* bst);

/**
 * Checks wheter or not the tree is empty
 */
bool BST_is_empty(BST* bst);

/**
 * Returns the compare function used in the tree
 */
void* BST_get_compare_func(BST* bst);

#endif
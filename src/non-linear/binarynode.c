/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/non-linear/binarynode.h"
#include <string.h>

 /**
  * Struct that implements a binary node, that can be used in binary trees
  *
  * This has a generic type value, a left child (binary node), and a right child (binary node)
  */
typedef struct binarynode {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Pointer to a generic type of data */
	void* ptr;

	/* Pointer to the binary node's father */
	binarynode* father;

	/* Pointer to the binary node's left child */
	binarynode* left;

	/* Pointer to the binary node's right child */
	binarynode* right;
} binarynode;

/**
 * Creates a binary node that can store elements of the given size
 */
binarynode* binarynode_create(void* x, size_t element_size) {

	binarynode* bn = NULL;

	if (0 < element_size && element_size <= SIZE_MAX && x) {

		bn = (binarynode*)malloc(sizeof(binarynode));

		if (bn) {
		
			bn->ptr = malloc(element_size);

			if (bn->ptr) {

				memcpy(bn->ptr, x, element_size);
				bn->father = NULL;
				bn->left = NULL;
				bn->right = NULL;
			}
			else {

				free(bn);
				bn = NULL;
			}
		}
	}

	return bn;
}

/**
 * Deletes the given binary node
 */
void binarynode_delete(binarynode** bn) {

	if (bn && *bn) {

		free((*bn)->ptr);
		memset(*bn, 0, sizeof(binarynode));
		free(*bn);
		*bn = NULL;
	}
	return;
}

/**
 * Returns the stored value
 */
void* binarynode_get_value(binarynode* bn) {

	return bn ? bn->ptr : NULL;
}

/**
 * Sets the second binary node as father of the first
 */
void binarynode_set_father(binarynode* bn, binarynode* father) {

	if (bn) bn->father = father;
	return;
}

/**
 * Sets the second binary node as left child of the first
 */
void binarynode_set_left_child(binarynode* bn, binarynode* left) {

	if (bn) bn->left = left;
	return;
}

/**
 * Sets the second binary node as right child of the first
 */
void binarynode_set_right_child(binarynode* bn, binarynode* right) {

	if (bn) bn->right = right;
	return;
}

/**
 * Returns the father
 */
binarynode* binarynode_get_father(binarynode* bn) {

	return bn ? bn->father : NULL;
}

/**
 * Returns the left child
 */
binarynode* binarynode_get_left_child(binarynode* bn) {

	return bn ? bn->left : NULL;
}

/**
 * Returns the right child
 */
binarynode* binarynode_get_right_child(binarynode* bn) {

	return bn ? bn->right : NULL;
}

/**
 * Checks if the node is a leaf or not (0 - 1 and 2 children respectively)
 */
bool binarynode_is_leaf(binarynode* bn) {

	return bn ? (!bn->left && !bn->right) : false;
}

/**
 * Returns the height of the tree whose root is bn
 */
int binarynode_get_height(binarynode* bn) {

	return bn ? 1 + max(binarynode_get_height(bn->left), binarynode_get_height(bn->right)) : 0;
}

/**
 * Returns the balance of the tree whose root is bn
 */
int binarynode_get_balance(binarynode* bn) {

	return bn ? binarynode_get_height(bn->left) - binarynode_get_height(bn->right) : 0;
}
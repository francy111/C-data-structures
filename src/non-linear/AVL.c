#include <stdlib.h>
#include <string.h>
#include "../../include/non-linear/AVL.h"
#include "../../include/non-linear/BST.h"

binarynode* AVL_util_right_rotation(binarynode* z);
binarynode* AVL_util_left_rotation(binarynode* z);

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
typedef struct AVL {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Actual tree */
	BST* tree;
} AVL;

/**
 * Creates a binary node that can store elements of the given size
 */
AVL* AVL_create(size_t element_size, int (*compare)(void*, void*)) {

	AVL* avl = (AVL*)malloc(sizeof(AVL));;

	if (avl) {

		avl->tree = BST_create(element_size, compare);
		if (!avl->tree) {

			free(avl);
			avl = NULL;
		}
	}
	return avl;
}

/**
 * Deletes the given binary node
 */
void AVL_delete(AVL** avl) {

	if (avl && *avl) {

		BST_delete(&(*avl)->tree);
		free(*avl);
		*avl = NULL;
	}
}

/**
 * Inserts the value x in the BST, according to the compare function
 */
void AVL_insert(AVL* avl, void* x) {

	if (avl && x) {

		// This is the newly added node
		binarynode* n = binarynode_get_father(BST_insert(avl->tree, x));

		int (*cmp)(void*, void*) = BST_get_compare_func(avl->tree);

		int balance;
		
		binarynode* tmp = n;

		while (n) {
			balance = binarynode_get_balance(n);


			// Left inbalance
			if (balance > 1) {

				if (cmp(x, binarynode_get_value(binarynode_get_left_child(n))) < 0) {

					// LL (RR rotation)
					n = AVL_util_right_rotation(n);
				} 

				else {

					// LR (LR rotation)
					binarynode_set_left_child(n, AVL_util_left_rotation(binarynode_get_left_child(n)));
					n = AVL_util_right_rotation(n);
				}
			}

			// Right inbalance
			else if (balance < -1) {


				if (cmp(x, binarynode_get_value(binarynode_get_right_child(n))) > 0) {

					// RR (LL rotation)
					n = AVL_util_left_rotation(n);
				}

				else {

					// RL (RL rotation)
					binarynode_set_right_child(n, AVL_util_right_rotation(binarynode_get_right_child(n)));
					n = AVL_util_left_rotation(n);
				}
			}

			n = binarynode_get_father(n);
		}
	}
	return;
}

/**
 * Removes the value x from the BST, if it is present
 */
void AVL_remove(AVL* avl, void* x) {

	if (avl && x) {

		// This is the father of the deleted node
		binarynode* n = BST_remove(avl->tree, x);

		int (*cmp)(void*, void*) = BST_get_compare_func(avl->tree);

		int balance;

		while (n) {
			balance = binarynode_get_balance(n);


			// Left inbalance
			if (balance > 1) {

				if (cmp(x, binarynode_get_value(binarynode_get_left_child(n))) < 0) {

					// LL (RR rotation)
					n = AVL_util_right_rotation(n);
				}

				else {

					// LR (LR rotation)
					binarynode_set_left_child(n, AVL_util_left_rotation(binarynode_get_left_child(n)));
					n = AVL_util_right_rotation(n);
				}
			}

			// Right inbalance
			else if (balance < -1) {


				if (cmp(x, binarynode_get_value(binarynode_get_right_child(n))) > 0) {

					// RR (LL rotation)
					n = AVL_util_left_rotation(n);
				}

				else {

					// RL (RL rotation)
					binarynode_set_right_child(n, AVL_util_right_rotation(binarynode_get_right_child(n)));
					n = AVL_util_left_rotation(n);
				}
			}

			n = binarynode_get_father(n);
		}
	}
	return;
}

/**
 * Searches the value x in the BST, returns a pointer to it's node
 */
binarynode* AVL_search(AVL* avl, void* x) {

	return avl ? BST_search(avl->tree, x) : NULL;
}

/**
 * Checks wheter or not the value x is present in the tree
 */
bool AVL_contains(AVL* avl, void* x) {

	return (avl && x) ? AVL_search(avl, x) : false;
}

/**
 * Returns the minimum value in the tree
 */
void* AVL_min(AVL* avl) {

	return avl ? BST_min(avl->tree) : NULL;
}

/**
 * Copies the minimum value in the tree inside the buffer
 */
void AVL_min_2(AVL* avl, void* buf) {

	if (avl) BST_min_2(avl->tree, buf);
	return;
}

/**
 * Returns the maximum value in the tree
 */
void* AVL_max(AVL* avl) {

	return avl ? BST_max(avl->tree) : NULL;
}

/**
 * Copies the maximum value in the tree inside the buffer
 */
void AVL_max_2(AVL* avl, void* buf) {

	if (avl) BST_max_2(avl->tree, buf);
	return;
}

/**
 * Returns the predecessor of the given node inside the tree
 */
binarynode* AVL_predecessor(AVL* avl, binarynode* bn) {

	return (avl && bn) ? BST_predecessor(avl->tree, bn) : NULL;
}

/**
 * Returns the successor of the given node inside the tree
 */
binarynode* AVL_successor(AVL* avl, binarynode* bn) {

	return (avl && bn) ? BST_successor(avl->tree, bn) : NULL;
}

/**
 * Traverses the tree in preorder, applying the function callback to each element
 */
void AVL_traverse_preoder(AVL* avl, void (*callback)(void*)) {

	if (avl && callback) {

		BST_traverse_preoder(avl->tree, callback);
	}
	return;
}

/**
 * Traverses the tree in inorder, applying the function callback to each element
 */
void AVL_traverse_inoder(AVL* avl, void (*callback)(void*)) {

	if (avl && callback) {

		BST_traverse_inoder(avl->tree, callback);
	}
	return;
}

/**
 * Traverses the tree in postorder, applying the function callback to each element
 */
void AVL_traverse_postoder(AVL* avl, void (*callback)(void*)) {

	if (avl && callback) {

		BST_traverse_postoder(avl->tree, callback);
	}
	return;
}

/**
 * Returns the number of elements in the tree
 */
size_t AVL_get_size(AVL* avl) {

	return avl ? BST_get_size(avl->tree) : 0;
}

/**
 * Returns the size of elements stored in the tree
 */
size_t AVL_get_element_size(AVL* avl) {

	return avl ? BST_get_element_size(avl->tree) : 0;
}

/**
 * Removes each element from the tree, the struct itself is preserved
 */
void AVL_clear(AVL* avl) {

	if (avl) {

		BST_clear(avl->tree);
	}
	return;
}

/**
 * Checks wheter or not the tree is empty
 */
bool AVL_is_empty(AVL* avl) {

	return avl ? BST_is_empty(avl->tree) : false;
}

/**
 * Returns the compare function used in the tree
 */
void* AVL_get_compare_func(AVL* avl) {

	return avl ? BST_get_compare_func(avl->tree) : NULL;
}

/**
 * Returns the height of the tree avl
 */
size_t AVL_get_height(AVL* avl) {

	return avl ? BST_get_height(avl->tree) : 0;
}

/**
 * Performs a right rotation on the tree with root z
 */
binarynode* AVL_util_right_rotation(binarynode* z) {

	binarynode* y = binarynode_get_left_child(z);
	binarynode* t = binarynode_get_right_child(y);

	binarynode_set_right_child(y, z);
	binarynode_set_left_child(z, t);

	binarynode_set_father(t, z);
	binarynode_set_father(y, binarynode_get_father(z));
	binarynode_set_father(z, y);

	return y;
}

/**
 * Performs a left rotation on the tree with root z
 */
binarynode* AVL_util_left_rotation(binarynode* x) {

	binarynode* y = binarynode_get_right_child(x);
	binarynode* t = binarynode_get_left_child(y);

	binarynode_set_left_child(y, x);
	binarynode_set_right_child(x, t);

	binarynode_set_father(t, x);
	binarynode_set_father(y, binarynode_get_father(x));
	binarynode_set_father(x, y);

	return y;
}
#include <stdlib.h>
#include <string.h>
#include "../../include/non-linear/BST.h"
#include "../../include/linear/stack.h"

void BST_util_count(void* x);
size_t UTIL_COUNT;

/**
 * Struct that implements a binary search tree, a struct that is used to store and then search
 * elements, as the structure itself is ordered.
 *
 * A BST is a binary tree in which each node holds a value that is, greater than all values
 * stored in the left child's tree, and less than all the values in the right child's tree
 *
 * This has a generic type value, so a compare function needs to be given
 */
typedef struct BST {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Root of the tree */
	binarynode* root;

	/* Since the data type is generic, we need to know the size of the elements */
	size_t element_size;

	/* Function used to determine the order of values */
	int (*compare)(void*, void*);
} BST;

/**
 * Creates a binary node that can store elements of the given size
 */
BST* BST_create(size_t element_size, int (*compare)(void*, void*)) {

	BST* bst = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		if (compare) {

			bst = (BST*)malloc(sizeof(BST));
			if (bst) {

				bst->root = NULL;
				bst->element_size = element_size;
				bst->compare = compare;
			}
		}
	}
	return bst;
}

/**
 * Deletes the given binary node
 */
void BST_delete(BST** bst) {

	if (bst && *bst) {

		BST_clear(*bst);
		(*bst)->element_size = 0;
		(*bst)->compare = NULL;
		free(*bst);
		*bst = NULL;
	}
}

/**
 * Inserts the value x in the BST, according to the compare function
 */
binarynode* BST_insert(BST* bst, void* x) {

	binarynode* n = NULL;

	if (bst && x) {

		n = binarynode_create(x, bst->element_size);
		if (n) {

			if (!bst->root) {

				bst->root = n;
			} 
			else{

				binarynode* tmp = bst->root;
				binarynode* n_father = NULL;

				// Stop when the current node is a leaf
				while (tmp) {

					n_father = tmp;
					tmp = bst->compare(x, binarynode_get_value(tmp)) < 0 ? binarynode_get_left_child(tmp) : binarynode_get_right_child(tmp);
				}

				// n_father holds the last pointer to a non null node
				binarynode_set_father(n, n_father);
				if (bst->compare(x, binarynode_get_value(tmp)) < 0) binarynode_set_left_child(n_father, n);
				else binarynode_set_right_child(n_father, n);
			}
		}
	}
	return n;
}

/**
 * Removes the value x from the BST, if it is present
 */
binarynode* BST_remove(BST* bst, void* x) {

	binarynode* father = NULL;

	if (bst && x) {

		binarynode* x_node = BST_search(bst, x);
		father = binarynode_get_father(x_node);

		if (x_node) {


			// Has at least 1 child
			if (binarynode_get_left_child(x_node)) {

				// Has both children, need to find successor or predecessor
				if (binarynode_get_right_child(x_node)) {

					binarynode* successor = BST_successor(bst, x_node);
					if (successor) {

						binarynode* tmp = binarynode_get_right_child(successor);
						if (binarynode_get_left_child(binarynode_get_father(successor)) == successor)
							binarynode_set_left_child(binarynode_get_father(successor), tmp);
						else
							binarynode_set_right_child(binarynode_get_father(successor), tmp);

						if (tmp) binarynode_set_father(tmp, binarynode_get_father(successor));

						binarynode_set_left_child(successor, binarynode_get_left_child(x_node));
						binarynode_set_right_child(successor, binarynode_get_right_child(x_node));
						binarynode_set_father(successor, binarynode_get_father(x_node));

						if (binarynode_get_left_child(x_node)) binarynode_set_father(binarynode_get_left_child(x_node), successor);
						if (binarynode_get_right_child(x_node)) binarynode_set_father(binarynode_get_right_child(x_node), successor);

						if (!binarynode_get_father(x_node)) bst->root = successor;
						else if(binarynode_get_left_child(binarynode_get_father(x_node)) == x_node) binarynode_set_left_child(binarynode_get_father(x_node), successor);
						else binarynode_set_right_child(binarynode_get_father(x_node), successor);
					}
				}

				// Has only 1 children, connect child with father  (right is null, left is not)
				else {

					// Not removing root
					if (father) {

						// Understand if x_node is left or right child
						if (x_node == binarynode_get_left_child(father)) {

							binarynode_set_left_child(father, binarynode_get_left_child(x_node));
						}
						else {

							binarynode_set_right_child(father, binarynode_get_left_child(x_node));
						}
					}

					// Removing the root
					else {

						bst->root = binarynode_get_left_child(x_node);
					}
					binarynode_set_father(binarynode_get_left_child(x_node), father);
				}
			}

			// Could have 0 or 1 children
			else {


				// Has only 1 children, connect child with father (left is null, right is not)
				if (binarynode_get_right_child(x_node)) {

					// Not removing the root
					if (father) {

						// Understand if x_node is left or right child
						if (x_node == binarynode_get_left_child(father)) {

							binarynode_set_left_child(father, binarynode_get_right_child(x_node));
						}
						else {

							binarynode_set_right_child(father, binarynode_get_right_child(x_node));
						}
					}

					// Remove the root
					else {

						bst->root = binarynode_get_right_child(x_node);
					}
					binarynode_set_father(binarynode_get_right_child(x_node), father);
				}

				// Has 0 children, just delete node
				else {

					// If we are not removing the root
					if (father) {

						// Understand if x_node is left or right child
						if (x_node == binarynode_get_left_child(father)) binarynode_set_left_child(father, NULL);
						else binarynode_set_right_child(father, NULL);
					}

					// If we are removing the root
					else {
							
						bst->root = NULL;
					}
				}
			}
			
			father = binarynode_get_father(x_node);
			// Delete the node (physical deletion)
			binarynode_delete(&x_node);
		}
	}
	return father;
}

/**
 * Searches the value x in the BST, returns a pointer to it's node
 */
binarynode* BST_search(BST* bst, void* x) {

	binarynode* n = NULL;

	if (bst) {

		binarynode* n = bst->root;
		int diff = 0;

		while (n) {

			diff = bst->compare(x, binarynode_get_value(n));

			// Node found
			if (diff == 0) {

				break;
			}

			// x < cur, continue the search on the left subtree
			else if (diff < 0) {

				n = binarynode_get_left_child(n);
			}

			// x > cur, continue the search on the right subtree
			else {

				n = binarynode_get_right_child(n);
			}
		}
	}
	return n;
}

/**
 * Checks wheter or not the value x is present in the tree
 */
bool BST_contains(BST* bst, void* x) {

	return (bst && x) ? BST_search(bst, x) : false;
}

/**
 * Returns the minimum value in the tree
 */
void* BST_min(BST* bst) {

	void* val = NULL;

	// The smallest element is in the all far left
	if (bst) {

		binarynode* tmp = bst->root;

		// Until there is a left child (smaller values)
		while (binarynode_get_left_child(tmp)) tmp = binarynode_get_left_child(tmp);

		// The next node has a NULL left child, so it holds the smallest value itself
		val = binarynode_get_value(tmp);
	} 
	return val;
}

/**
 * Copies the minimum value in the tree inside the buffer
 */
void BST_min_2(BST* bst, void* buf) {

	// The smallest element is in the all far left
	if (bst) {

		binarynode* tmp = bst->root;

		// Until there is a left child (smaller values)
		while (binarynode_get_left_child(tmp)) tmp = binarynode_get_left_child(tmp);

		// The next node has a NULL left child, so it holds the smallest value itself
		memcpy(buf, binarynode_get_value(tmp), bst->element_size);
	}
	return;
}

/**
 * Returns the maximum value in the tree
 */
void* BST_max(BST* bst) {

	void* val = NULL;

	// The largest element is in the all far right
	if (bst) {

		binarynode* tmp = bst->root;

		// Until there is a right child (greater values)
		while (binarynode_get_right_child(tmp)) tmp = binarynode_get_right_child(tmp);

		// The next node has a NULL right child, so it holds the greatest value itself
		val = binarynode_get_value(tmp);
	}
	return val;
}

/**
 * Copies the maximum value in the tree inside the buffer
 */
void BST_max_2(BST* bst, void* buf) {

	// The largest element is in the all far right
	if (bst) {

		binarynode* tmp = bst->root;

		// Until there is a right child (greater values)
		while (binarynode_get_right_child(tmp)) tmp = binarynode_get_right_child(tmp);

		// The next node has a NULL right child, so it holds the greatest value itself
		memcpy(buf, binarynode_get_value(tmp), bst->element_size);
	}
	return;
}

/**
 * Returns the predecessor of the given node inside the tree
 */
binarynode* BST_predecessor(BST* bst, binarynode* bn) {

	binarynode* prev = NULL;

	if (bst && bn) {

		// If there is the left sub-tree, the successor is the element the most to the right in the left subtree
		if (binarynode_get_left_child(bn)) {

			binarynode* tmp = binarynode_get_left_child(bn);
			while (binarynode_get_right_child(bn)) bn = binarynode_get_right_child(bn);

			prev = tmp;
		}

		// Otherwise, we have to go up until we find a node that is the right child of its father
		else {

			binarynode* father = binarynode_get_father(bn);
			while (father && bn == binarynode_get_left_child(father)) {
			
				bn = father;
				father = binarynode_get_father(father);
			}

			prev = father;
		}
	}

	return prev;
}

/**
 * Returns the successor of the given node inside the tree
 */
binarynode* BST_successor(BST* bst, binarynode* bn) {

	binarynode* succ = NULL;

	if (bst && bn) {

		// If there is the right sub-tree, the successor is the element the most to the left in the right subtree
		if (binarynode_get_right_child(bn)) {

			binarynode* tmp = binarynode_get_right_child(bn);
			while (binarynode_get_left_child(bn)) bn = binarynode_get_left_child(bn);

			succ = tmp;
		}

		// Otherwise, we have to go up until we find a node that is the left child of its father
		else {

			binarynode* father = binarynode_get_father(bn);
			while (father && bn == binarynode_get_right_child(father)) {

				bn = father;
				father = binarynode_get_father(father);
			}

			succ = father;
		}
	}

	return succ;
}

/**
 * Traverses the tree in preorder, applying the function callback to each element
 */
void BST_traverse_preoder(BST* bst, void (*callback)(void*)) {

	if (bst && callback) {
	
		stack* stck = stack_create(sizeof(binarynode*));
		if (stck) {
		
			stack_push(stck, bst->root);
			binarynode* tmp = NULL;

			while (!stack_is_empty(stck)) {

				tmp = (binarynode*)stack_pop(stck);

				// Visit current node
				callback(binarynode_get_value(tmp));

				// Insert right child first so the left one is elaborated first
				if (binarynode_get_right_child(tmp)) stack_push(stck, binarynode_get_right_child(tmp));
				if (binarynode_get_left_child(tmp)) stack_push(stck, binarynode_get_left_child(tmp));
				free(tmp);
			}

			stack_delete(&stck);
		}
	}
	return;
}

/**
 * Traverses the tree in inorder, applying the function callback to each element
 */
void BST_traverse_inoder(BST* bst, void (*callback)(void*)) {

	if (bst && callback) {

		stack* stck = stack_create(sizeof(binarynode*));
		if (stck) {

			binarynode* tmp = bst->root;
			binarynode* used_to_del = NULL;
			while (tmp || !stack_is_empty(stck)) {

				// Go to the far left
				while (tmp) {

					stack_push(stck, tmp);
					tmp = binarynode_get_left_child(tmp);
				}

				tmp = stack_pop(stck);
				used_to_del = tmp;
				callback(binarynode_get_value(tmp));
				tmp = binarynode_get_right_child(tmp);
				free(used_to_del);
			}
			stack_delete(&stck);
		}
	}
	return;
}

/**
 * Traverses the tree in postorder, applying the function callback to each element
 */
void BST_traverse_postoder(BST* bst, void (*callback)(void*)) {

	if (bst && callback) {

		stack* preoder_emulator = stack_create(sizeof(binarynode*));
		stack* actual_postorder = stack_create(bst->element_size);

		if (preoder_emulator && actual_postorder) {

			stack_push(preoder_emulator, bst->root);
			binarynode* tmp = NULL;

			while (!stack_is_empty(preoder_emulator)) {

				tmp = (binarynode*)stack_pop(preoder_emulator);
				stack_push(actual_postorder, binarynode_get_value(tmp));

				// Left first, so the order is correct when inverting
				if (binarynode_get_left_child(tmp)) stack_push(preoder_emulator, binarynode_get_left_child(tmp));
				if (binarynode_get_right_child(tmp)) stack_push(preoder_emulator, binarynode_get_right_child(tmp));
				free(tmp);
			}

			void* val = NULL;
			while (!stack_is_empty(actual_postorder)) {
			
				val = stack_pop(actual_postorder);
				callback(val);
				free(val);
			}

		}
		stack_delete(&preoder_emulator);
		stack_delete(&actual_postorder);
	}
	return;
}

/**
 * Returns the number of elements in the tree
 */
size_t BST_get_size(BST* bst) {

	size_t dim = 0;

	if (bst) {

		UTIL_COUNT = 0;
		BST_traverse_preoder(bst, *BST_util_count);
		dim = UTIL_COUNT;
	}
	return dim;
}

/**
 * Returns the size of elements stored in the tree
 */
size_t BST_get_element_size(BST* bst) {

	return bst ? bst->element_size : 0;
}

/**
 * Removes each element from the tree, the struct itself is preserved
 */
void BST_clear(BST* bst) {

	if (bst) {

		while (bst->root) {

			BST_remove(bst, binarynode_get_value(bst->root));
		}
	}
	return;
}

/**
 * Checks wheter or not the tree is empty
 */
bool BST_is_empty(BST* bst) {

	return bst ? (bst->root == NULL) : false;
}

/**
 * Returns the compare function used in the tree
 */
void* BST_get_compare_func(BST* bst) {

	return bst ? bst->compare : NULL;
}

/**
 * Returns the height of the tree bst
 */
size_t BST_get_height(BST* bst) {

	return bst ? binarynode_get_height(bst->root) : 0;
}

/**
 * Accepts a pointer to a counter and updates it
 */
void BST_util_count(void* x) {

	UTIL_COUNT += x ? 1 : 0;
}
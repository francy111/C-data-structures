/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/skiplist.h"
#include "../../include/linear/snode.h"
#include <string.h>

/* Utility function used to generate a level for a node */
size_t sl_generate_random_level(double probability, size_t max_levels);

/**
 * Struct that represent a list of elements of a generic type value
 */
typedef struct skiplist {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	 /* Pointer to the first node of the list, which is in the first level, in this case, a sentinel node */
	snode* sentinel;

	/* Number of elements present in the list */
	size_t element_count;

	/* Size of the elements stored in the list */
	size_t element_size;

	/* Maximum number of levels the nodes can be in */
	size_t max_levels;

	/* Probability used to determine the levels of each node */
	double probability;

	/* Function used to compare elements to determine the order within the list */
	int (*compare)(void*, void*);
} skiplist;

/**
 *  Creates a linked list ready to store elements that are as big as the given size
 */
skiplist* sl_create(size_t element_size, size_t max_levels, double probability, int (*cmp)(void*, void*)) {

	skiplist* sl = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		if (0 < max_levels && max_levels <= SIZE_MAX) {

			if (0.0 < probability && probability < 1.0) {
				if (cmp) {

					sl = (skiplist*)malloc((sizeof(skiplist)));
					if (sl) {

						sl->element_count = 0;
						sl->element_size = element_size;
						sl->max_levels = max_levels;
						sl->compare = cmp;
						sl->probability = probability;

						void* sentinel_val = malloc(sl->element_size);
						if (sentinel_val) {
							memset(sentinel_val, 0, sl->element_size);
							sl->sentinel = snode_create(sentinel_val, sl->element_size, sl->max_levels);

							if (!sl->sentinel) {

								free(sl);
								sl = NULL;
							}
							free(sentinel_val);
						}
						else {

							free(sl);
							sl = NULL;
						}
					}
				}
			}
		}
	}
	return sl;
}

/**
 * Deletes the given list
 */
void sl_delete(skiplist** sl) {

	// Access the list only if the pointer is valid
	if (sl && *sl) {

		// Free every node
		sl_clear(*sl);

		// Free the memory used for the whole struct
		memset(*sl, 0, sizeof(skiplist));
		free(*sl);
		*sl = NULL; 
	}
	return;
}

/**
 * Insert the element pointed to by x in the skiplist
 */
void sl_insert(skiplist* sl, void* x) {

	if (sl && x) {

		snode** update = (snode**)malloc(sizeof(snode*) * sl->max_levels);
		if (update) {

			snode* tmp = sl->sentinel;

			for (size_t i = sl->max_levels; i > 0; i--) {

				while (snode_get_next(tmp, i - 1) && (sl->compare(snode_get_value(snode_get_next(tmp, i - 1)), x) < 0)) {

					tmp = snode_get_next(tmp, i - 1);
				}
				update[i - 1] = tmp;
			}

			size_t node_level = sl_generate_random_level(sl->probability, sl->max_levels);
			snode* to_be_inserted = snode_create(x, sl->element_size, node_level);
			if (to_be_inserted) {

				for (size_t i = 0; i < node_level; i++) {

					snode_set_next(to_be_inserted, snode_get_next(update[i], i), node_level);
					snode_set_next(update[i], to_be_inserted, i);
				}
			}
			free(update);
			sl->element_count++;
			if (node_level > sl->max_levels)
				sl->max_levels = node_level;
		}
	}
	return;
}

/**
 * Removes x from the list (if present)
 */
void sl_remove(skiplist* sl, void* x) {

	if (sl && x) {

		snode** update = (snode**)malloc(sizeof(snode*) * sl->max_levels);
		if (update) {

			snode* tmp = sl->sentinel;

			for (size_t i = sl->max_levels; i > 0; i--) {

				while (snode_get_next(tmp, i - 1) && (sl->compare(snode_get_value(snode_get_next(tmp, i - 1)), x) < 0)) {

					tmp = snode_get_next(tmp, i - 1);
				}
				update[i - 1] = tmp;
			}

			snode* to_be_deleted = snode_get_next(tmp, 0);

			if (to_be_deleted && (sl->compare(snode_get_value(to_be_deleted), x) == 0)) {

				for (size_t i = 0; i < sl->max_levels; i++) {

					if (snode_get_next(update[i], i) != to_be_deleted) break;
					snode_set_next(update[i], snode_get_next(to_be_deleted, i), i);

					snode_delete(&to_be_deleted);

					while (sl->max_levels > 1 && !snode_get_next(sl->sentinel, sl->max_levels - 1))
						sl->max_levels--;
				}
			}
			free(update);
			sl->element_count--;
		}
	}
	return;
}

/**
 * Returns a pointer to the i -th element of the list
 */
void* sl_search(skiplist* sl, void* x) {
	
	snode* to_ret = NULL;

	if (sl && x) {
	
		to_ret = sl->sentinel;
		for (size_t i = sl->max_levels; i > 0; i++) {

			while (snode_get_next(to_ret, i) && (sl->compare(snode_get_value(snode_get_next(to_ret, i)), x) < 0))
				to_ret = snode_get_next(to_ret, i);
		}

		to_ret = snode_get_next(to_ret, 0);
	}

	return (to_ret && (sl->compare(snode_get_value(to_ret), x) == 0)) ? to_ret : NULL;
}

/**
 * Returns the number of elements of the list
 */
size_t sl_get_size(skiplist* sl) {

	return sl ? sl->element_count : 0;
}

/**
 * Returns the size of the elements of the list
 */
size_t sl_get_element_size(skiplist* sl) {

	return sl ? sl->element_size : 0;
}

/**
 * Returns the number of levels in the skip list
 */
size_t sl_get_max_levels(skiplist* sl) {

	return sl ? sl->max_levels : 0;
}

/**
 * Checks if the element pointed to by x is present in the list
 */
bool sl_contains(skiplist* sl, void* x) {

	return (sl && x) ? sl_search(sl, x) : false;
}

/**
 * Checks whether the list contains at least one element or not
 */
bool sl_is_empty(skiplist* sl) {

	return sl ? !(sl->element_count) : false;
}

/**
 * Removed every element from the list
 * (the list struct itself is not deleted)
 */
void sl_clear(skiplist* sl) {

	if (sl) {

		snode* tmp = snode_get_next(sl->sentinel, 0);
		snode* next = NULL;

		while(tmp) {

			next = snode_get_next(tmp, 0);
			snode_delete(&tmp);
			tmp = next;
		}

		for (size_t i = 0; i < sl->max_levels; i++) {

			snode_set_next(sl->sentinel, NULL, i);
		}
	}
	return;
}

/* Utility function to calculate node level */
size_t sl_generate_random_level(double probability, size_t max_levels) {

	size_t level = 1;

	while (((rand() / (double)RAND_MAX) < probability) && (level < max_levels))
		level++;

	return level;
}
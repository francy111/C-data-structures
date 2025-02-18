/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef GRAPH_WITH_ADJACENCY_LIST

#include "../../include/non-linear/graph.h"
#include "../../include/linear/linkedlist.h"
#include "../../include/linear/queue.h"
#include "../../include/linear/stack.h"
#include <string.h>
#include <stdbool.h>

typedef struct gnode {

	void* value;
	linkedlist* adjacency;
}gnode;

/**
 * Struct that represent a graph that can store
 * nodes of  ageneric data type
 */
typedef struct graph {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	 /* List of lists of nodes */
	linkedlist* nodes;

	/* Size of the elements stored in the list */
	size_t element_size;

	/* Flags for weighted and oriented graphs */
	int flags;

} graph;

/**
 * Creates a graph that will store elements that are as big as the  given size
 */
graph* graph_create(size_t element_size, int flags) {

	graph* g = NULL;

	if (0 < element_size && element_size <= SIZE_MAX) {

		g = (graph*)malloc(sizeof(graph));
		if (g) {

			g->nodes = ll_create(sizeof(gnode));
			if (g->nodes) {

				g->element_size = element_size;
				g->flags = flags;
			}
			else {

				free(g);
				g = NULL;
			}
		}
	}
	return g;
}

/**
 * Deletes the given graph
 */
void graph_delete(graph** g) {

	if (g && *g) {

		graph_clear_nodes(*g);
		memset(*g, 0, sizeof(graph));
		free(*g);
		*g = NULL;
	}
	return;
}

/**
 * Inserts a node in the graph that will hold the value x
 */
void graph_insert_node(graph* g, void* x) {

	if (g && x) {

		// If node is not contained, add it
		if (graph_search_node(g, x) == NULL) {

			gnode* to_add = (gnode*)malloc(sizeof(gnode));
			if (to_add) {

				to_add->value = x;
				to_add->adjacency = ll_create(g->element_size);

				if (to_add->adjacency) {

					ll_insert_tail(g->nodes, to_add);
				}
				else {

					free(to_add);
				}
			}
		}

	}
	return;
}

/**
 * Connects the two given nodes with the given weight (= inserts an arch)
 */
void graph_insert_arch(graph* g, void* first, void* second, int weight) {

	if (g && first && second && weight > 0) {

		// If both elements are present
		if (graph_search_node(g, first) && graph_search_node(g, second)) {

			// Graph search arch will either return the pointer to the list of adjacency of first, if the arch is present, or null if it's not
			void* adjacency_list = graph_search_arch(g, first, second);
			if (!adjacency_list) {

				// Need to add the arch
				linkedlist* ll = ((gnode*)graph_search_node(g, first))->adjacency;
				ll_insert_tail(ll, second);
			}

			// If the graph is NOT oriented
			if (g->flags & ~IS_ORIENTED) {

				adjacency_list = graph_search_arch(g, second, first);
				if (!adjacency_list) {

					// Need to add the arch
					linkedlist* ll = ((gnode*)graph_search_node(g, second))->adjacency;
					ll_insert_tail(ll, first);
				}
			}
		}

	}
	return;
}

/**
 * Removes the node that holds x from the graph
 */
void graph_remove_node(graph* g, void* x) {


	if (g && x) {

		gnode* node = graph_search_node(g, x);

		// The element is present
		if (node) {

			ll_delete(&(node->adjacency));
			ll_remove_at(g->nodes, -1 + ll_contains(g->nodes, node));

			// For each element in the list, check if the arch exists and remove it
			gnode* tmp = NULL;
			for (int i = 0; i < ll_get_size(g->nodes); i++) {

				tmp = ll_get_at(g->nodes, i);

				// If an arch exists
				if (graph_search_arch(g, tmp->value, x)) {

					ll_remove_at(tmp->adjacency, ll_contains(tmp->adjacency, x) - 1);
				}
			}
		}
	}
	return;
}

/**
 * Removes the arch that connects the two nodes
 */
void graph_remove_arch(graph* g, void* first, void* second) {

	if (g && first && second) {

		if (first != second) {

			// If both are present
			gnode* of_first = graph_search_node(g, first);
			gnode* of_second = graph_search_node(g, second);
			if (of_first && of_second) {

				// If the adjacency list of the first element contains the second, first->second exists
				int k = -1;
				if ((k = ll_contains(of_first->adjacency, second))) {

					ll_remove_at(of_first->adjacency, k - 1);
				}

			
				// If the graph is not oriented, we can also try second->first
				if (g->flags & ~IS_ORIENTED) {

					int k = -1;
					if ((k = ll_contains(of_second->adjacency, first))) {

						ll_remove_at(of_second->adjacency, k - 1);
					}
				}
			}
		}
	}
	return;
}

/**
 * Searches for the value x in the graph and returns a pointer to it (if present)
 */
void* graph_search_node(graph* g, void* x) {

	void* val = NULL;

	if (g && x) {

		gnode* tmp = NULL;
		for (int i = 0; i < ll_get_size(g->nodes); i++) {

			tmp = ll_get_at(g->nodes, i);
			if (tmp->value == x) {

				val = tmp;
				break;
			}
		}
	}
	return val;
}

/**
 * Searches for an arch between first and second
 */
void* graph_search_arch(graph* g, void* first, void* second) {

	void* val = NULL;

	if (g && first && second) {

		if (first != second) {

			gnode* of_first = graph_search_node(g, first);
			gnode* of_second = graph_search_node(g, second);
			if (of_first && of_second) {

				// If the adjacency list of the first element contains the second, first->second exists
				int k = -1;
				if ((k = ll_contains(of_first->adjacency, second))) {

					val = ll_get_at(of_first->adjacency, k - 1);
				}

				// first->second doesn't exist
				else {
					
					// If the graph is not oriented, we can also try second->first
					if (g->flags & ~IS_ORIENTED) {

						int k = -1;
						if ((k = ll_contains(of_second->adjacency, first))) {

							val = ll_get_at(of_second->adjacency, k - 1);
						}
					}
				}
			}
		}
	}
	return val;
}

/**
 * Traverses the graph breadth-first and applies the callback function to each element
 */
void graph_BFS(graph* g, void (*callback)(void*)) {

	if (g && callback) {

		bool* visited = (bool*)calloc(ll_get_size(g->nodes), sizeof(bool));
		queue* q = queue_create(sizeof(gnode));

		if (visited && q) {

			gnode* cur = ll_get_head(g->nodes);
			queue_enqueue(q, cur);
			visited[ll_contains(g->nodes, cur) - 1] = true;

			while (!queue_is_empty(q)) {

				cur = queue_dequeue(q);
				callback(cur->value);
				int index = 0;
				for (int i = 0; i < ll_get_size(cur->adjacency); i++) {

					index = ll_contains(g->nodes, ll_get_at(cur->adjacency, i)) - 1;
					if(!visited[index]) {

						queue_enqueue(q, ll_get_at(cur->adjacency, i));

					}
				}
			}
		}
	}
	return;
}

/**
 * Traverses the graph depths-first and applies the callback function to each element
 */
void graph_DFS(graph* g, void (*callback)(void*)) {

	if (g && callback) {

		bool* visited = (bool*)calloc(ll_get_size(g->nodes), sizeof(bool));
		stack* s = stack_create(sizeof(gnode));

		if (visited && s) {

			gnode* cur = ll_get_head(g->nodes);
			stack_push(s, cur);
			visited[ll_contains(g->nodes, cur) - 1] = true;

			while (!stack_is_empty(s)) {

				cur = stack_pop(s);
				callback(cur->value);
				int index = 0;
				for (int i = 0; i < ll_get_size(cur->adjacency); i++) {

					index = ll_contains(g->nodes, ll_get_at(cur->adjacency, i)) - 1;
					if (!visited[index]) {

						stack_push(s, ll_get_at(cur->adjacency, i));

					}
				}
			}
		}
	}
	return;
}

/**
 * Removes all nodes (all the arches aswell)
 */
void graph_clear_nodes(graph* g) {

	if (g) {

		graph_clear_arches(g);
		ll_clear(g->nodes);
	}
	return;
}

/**
 * Removes all the arches
 */
void graph_clear_arches(graph* g) {

	if (g) {

		// Traverse the list of couples (value + list) and delete the list

		void* tmp;
		for (int i = 0; i < ll_get_size(g->nodes); i++) {

			tmp = ll_get_at(g->nodes, i);

			// tmp points to a memory location that contains [value, pointer to list], increment it
			tmp = (char*)tmp + g->element_size;

			// tmp now points to a location that contains a pointer to a list, so
			ll_clear((linkedlist*)tmp);
		}
	}
	return;
}

#endif
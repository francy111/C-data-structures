/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#define GRAPH_WITH_EDGE_LIST
#ifdef GRAPH_WITH_EDGE_LIST

#include "../../include/non-linear/graph.h"
#include "../../include/linear/linkedlist.h"
#include "../../include/linear/queue.h"
#include "../../include/linear/stack.h"
#include <string.h>
#include <stdbool.h>

typedef struct edge {

	void* first_node;
	void* second_node;
	int weight;
} edge;

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

	 /* List of lists of edges */
	linkedlist* edges;

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

			g->edges = ll_create(sizeof(edge));
			if (g->edges) {

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

		// If the node is present, then we don't insert it
		if (!graph_search_node(g, x)) {

			edge* ed = (edge*)malloc(sizeof(edge));
			if (ed) {

				ed->first_node = x;
				ed->second_node = x;
				ed->weight = 0;
				ll_insert_tail(g->edges, ed);
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

		// If the arch is not present, insert it
		if (!graph_search_arch(g, first, second)) {

			edge* ed = (edge*)malloc(sizeof(edge));
			if (ed) {

				ed->first_node = first;
				ed->second_node = second;
				ed->weight = weight;
				ll_insert_tail(g->edges, ed);
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

		graph_remove_arch(g, x, x);
	}
	return;
}

/**
 * Removes the arch that connects the two nodes
 */
void graph_remove_arch(graph* g, void* first, void* second) {

	if (g && first && second) {

		edge* to_be_deleted = graph_search_arch(g, first, second);
		if (to_be_deleted) {

			// Removing the self arch implies removing the node altogether
			if (to_be_deleted->first_node == to_be_deleted->second_node) {

				// Remove all of its edges
				edge* tmp = NULL;
				for (int i = 0; i < ll_get_size(g->edges); i++) {

					tmp = ll_get_at(g->edges, i);
					if (tmp->first_node == to_be_deleted->first_node || tmp->second_node == to_be_deleted->first_node) {

						ll_remove_at(g->edges, i);
					}
				}
			}
			else {

				ll_remove_at(g->edges, -1 + ll_contains(g->edges, to_be_deleted));
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

		val = graph_search_arch(g, x, x);

	}
	return val;
}

/**
 * Searches for an arch between first and second
 */
void* graph_search_arch(graph* g, void* first, void* second) {

	void* val = NULL;

	if (g && first && second) {

		edge* current = NULL;
		for (int i = 0; i < ll_get_size(g->edges); i++) {

			current = ll_get_at(g->edges, i);

			// Check if it's the same edge
			if (first == current->first_node && second == current->second_node) {

				val = current;
				break;
			}

			// In case the graph is not oriented, we can also try to check for second->first
			else {

				if (g->flags & ~IS_ORIENTED && first == current->second_node && second == current->first_node) {
				
					val = current;
					break;
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

	// Not currently implemented
	return;
}

/**
 * Traverses the graph depths-first and applies the callback function to each element
 */
void graph_DFS(graph* g, void (*callback)(void*)) {

	// Not currently implemented
	return;
}

/**
 * Removes all nodes (all the arches aswell)
 */
void graph_clear_nodes(graph* g) {

	if (g) {
		
		// Since we memorize edges, we have to remove everything
		ll_clear(g->edges);
	}
	return;
}

/**
 * Removes all the arches
 */
void graph_clear_arches(graph* g) {

	if (g) {

		// We just need to remove edges, so we don't remove the edges (x,x) (self arch, means the node exists)
		
		edge* tmp = NULL;
		for (int i = 0; i < ll_get_size(g->edges); i++) {

			tmp = (edge*)ll_get_at(g->edges, i);
			if (tmp->first_node != tmp->second_node)
				ll_remove_at(g->edges, i);
		}
	}
	return;
}

#endif
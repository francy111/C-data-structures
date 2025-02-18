/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef GRAPH_WITH_MATRIX

#include "../../include/non-linear/graph.h"
#include "../../include/linear/linkedlist.h"
#include "../../include/linear/queue.h"
#include "../../include/linear/stack.h"
#include <string.h>
#include <stdbool.h>

#define MAX_ELEMENTS 100

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

	/* List of nodes */
	linkedlist* nodes;

	/* Matrix of adjacency, used to represent the arches */
	int adjacency_matrix[MAX_ELEMENTS][MAX_ELEMENTS];

	/* Number of current elements in the graph */
	int current_elements;

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

			g->nodes = ll_create(element_size);
			if (g->nodes) {

				g->current_elements = 0;
				g->element_size = element_size;
				g->flags = flags;
				for (int i = 0; i < MAX_ELEMENTS; i++)
					for (int j = 0; j < MAX_ELEMENTS; j++)
						g->adjacency_matrix[i][j] = 0;
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

		ll_delete(&(*g)->nodes);
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

	if(g && x) {
	
		if (g->current_elements < MAX_ELEMENTS && !ll_contains(g->nodes, x)) {

			ll_insert_tail(g->nodes, x);
			g->current_elements++;

			// Initialize the row/column for the matrix
			for (int i = 0; i < g->current_elements; i++)
				g->adjacency_matrix[i][g->current_elements - 1] = 0;
			for (int j = 0; j < g->current_elements; j++)
				g->adjacency_matrix[g->current_elements - 1][j] = 0;
		}
	}
	return;
}

/**
 * Connects the two given nodes with the given weight (= inserts an arch)
 */
void graph_insert_arch(graph* g, void* first, void* second, int weight) {

	if (g && first && second && weight > 0) {
	
		if (first != second) {

			// Search for the indexes of the elements
			int i = ll_contains(g->nodes, first), j = ll_contains(g->nodes, second);

			if (i > 0 && j > 0) {

				g->adjacency_matrix[i - 1][j - 1] = (g->flags & IS_WEIGHTED) ? weight : 1;
				if (g->flags & ~IS_ORIENTED) g->adjacency_matrix[j - 1][i - 1] = (g->flags & IS_WEIGHTED) ? weight : 1;
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

		// Search for the indexes of the element
		int k = ll_contains(g->nodes, x);

		if (k > 0) {

			// Adjust index
			k--;

			ll_remove_at(g->nodes, k);

			// Free the row and column occupied by the old node
			for (int i = 0; i < g->current_elements; i++)
				g->adjacency_matrix[i][k] = 0;
			for (int j = 0; j < g->current_elements; j++)
				g->adjacency_matrix[k][j] = 0;

			// Shift each row after the deleted one 1 up
			for (int i = k + 1; i < g->current_elements; i++) {

				for (int j = 0; j < g->current_elements; j++) {

					if (j != k)
						g->adjacency_matrix[i - 1][j] = g->adjacency_matrix[i][j];
				}
			}

			// Shift each column after the deleted one 1 to the left
			for (int j = k + 1; j < g->current_elements; j++) {

				for (int i = 0; i < g->current_elements; i++) {

					if (i != k)
						g->adjacency_matrix[i][j - 1] = g->adjacency_matrix[i][j];
				}
			}

			// The last row and index are to be deleted now
			for (int i = 0; i < g->current_elements; i++)
				g->adjacency_matrix[i][g->current_elements - 1] = 0;
			for (int j = 0; j < g->current_elements; j++)
				g->adjacency_matrix[g->current_elements - 1][j] = 0;

			g->current_elements--;
		}
	}
	return;
}

/**
 * Removes the arch that connects the two nodes
 */
void graph_remove_arch(graph* g, void* first, void* second) {

	if (g && first && second) {
		
		// Search for the indexes of the elements
		int i = ll_contains(g->nodes, first), j = ll_contains(g->nodes, second);

		if (i > 0 && j > 0) {

			g->adjacency_matrix[i - 1][j - 1] = 0;
			if (g->flags & ~IS_ORIENTED) g->adjacency_matrix[j - 1][i - 1] = 0;
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

		int index = ll_contains(g->nodes, x);
		if (index) val = ll_get_at(g->nodes, index - 1);
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

			int i = ll_contains(g->nodes, first);
			int j = ll_contains(g->nodes, second);
			if (i > 0 && j > 0) {

				val = &(g->adjacency_matrix[i - 1][j - 1]);
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

		bool visited[MAX_ELEMENTS] = { false };
		queue* q = queue_create(sizeof(void*));

		if (q) {

			void* cur = ll_get_head(g->nodes);
			queue_enqueue(q, cur);
			visited[ll_contains(g->nodes, cur) - 1] = true;

			while (!queue_is_empty(q)) {

				cur = queue_dequeue(q);
				callback(cur);
				for (int i = 0; i < ll_get_size(g->nodes); i++) {

					if (g->adjacency_matrix[ll_contains(g->nodes, cur) - 1][i] && !visited[i]) {

						queue_enqueue(q, ll_get_at(g->nodes, i));
						visited[i] = true;
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

		bool visited[MAX_ELEMENTS] = { false };
		stack* s = stack_create(sizeof(void*));

		if (s) {

			void* cur = ll_get_head(g->nodes);
			stack_push(s, cur);
			visited[ll_contains(g->nodes, cur) - 1] = true;

			while (!stack_is_empty(s)) {

				cur = stack_pop(s);
				callback(cur);
				for (int i = 0; i < ll_get_size(g->nodes); i++) {

					if (g->adjacency_matrix[ll_contains(g->nodes, cur) - 1][i] && !visited[i]) {

						stack_push(s, ll_get_at(g->nodes, i));
						visited[i] = true;
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
		g->current_elements = 0;
	}
	return;
}

/**
 * Removes all the arches
 */
void graph_clear_arches(graph* g) {

	if(g) {

		for (int i = 0; i < g->current_elements; i++)
			for (int j = 0; j < g->current_elements; j++)
				g->adjacency_matrix[i][j] = 0;
	}
	return;
}

#endif
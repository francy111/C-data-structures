/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef GRAPH__H
#define GRAPH__H

#define IS_WEIGHTED (1 << 0)
#define IS_ORIENTED (1 << 1)

#include <stdlib.h>

/**
 * Struct that represent a graph that can store
 * nodes of  ageneric data type
 */
typedef struct graph graph;

/**
 * Creates a graph that will store elements that are as big as the  given size
 */
graph* graph_create(size_t element_size, int flags);

/**
 * Deletes the given graph
 */
void graph_delete(graph** g);

/**
 * Inserts a node in the graph that will hold the value x
 */
void graph_insert_node(graph* g, void* x);

/**
 * Connects the two given nodes with the given weight (= inserts an arch)
 */
void graph_insert_arch(graph* g, void* first, void* second, int weight);

/**
 * Removes the node that holds x from the graph
 */
void graph_remove_node(graph* g, void* x);

/**
 * Removes the arch that connects the two nodes
 */
void graph_remove_arch(graph* g, void* first, void* second);

/**
 * Searches for the value x in the graph and returns a pointer to it (if present)
 */
void* graph_search_node(graph* g, void* x);

/**
 * Searches for an arch between first and second
 */
void* graph_search_arch(graph* g, void* first, void* second);

/**
 * Traverses the graph breadth-first and applies the callback function to each element
 */
void graph_BFS(graph* g, void (*callback)(void*));

/**
 * Traverses the graph depths-first and applies the callback function to each element
 */
void graph_DFS(graph* g, void (*callback)(void*));

/**
 * Removes all nodes (all the arches aswell)
 */
void graph_clear_nodes(graph* g);

/**
 * Removes all the arches
 */
void graph_clear_arches(graph* g);

#endif
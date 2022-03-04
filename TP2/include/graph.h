#pragma once

#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "queue.h"
#include "stack.h"

/**
 *
 * @file
 * @brief This is my new file
 *
 */

/**
 * @brief Implementation of the Graph data structure using an adjacency list.
 *
 */
typedef struct {

    AdjacencyList adj;
    int nedge;
    int nvert;
    bool di;

} Graph;

// So a graphe is a structure that contains edges E (sommets) and vertices V (arretes ou arcs)
// G = < V, E >

// What's the best way to encode this structure in C?
//
// Since this is a simple graph

// By convention, we use the names 0 through V-1 for the vertices in a V-vertex graph.

// From Sedgewick's Algorithms,
// There are effectively three ways to represent a graph.
//
// As an adjacency matrix,
// an array of edges,
// an array of adjacency lists.

// Let's implement all three types but we will ultimately be using the array of adjacency list.

// First off, the adjacency array will me an array of vertices 0, V-1

// We are going to assume that the elements are integers. we will make this "generic" code by including
// a preprocessor graph type

/**
 * @brief Allocate space for a new Graph
 *
 * @param __nv Number of vertices
 * @param __ne Number of edges
 * @param __digraph True if the graph is oriented
 * @return Graph* A pointer to the newly allocated Graph structure
 */
Graph *newGraph(int __nv, int __ne, bool __digraph);

// replicate the connections of graph __G and return a newly allocated graph
/**
 * @brief Perform a deep copy of a graph
 *
 * Create a new graph whose connections are exactly the same as __G, but whose adjacency list
 * is full of newly allocated nodes. This way the new graph has complete control of the elements that
 * it points to
 *
 * @param __G The Graph to duplicate
 * @return A pointer to the newly allocated copy
 */
Graph *duplicateGraph(const Graph *__G);

// Free all of the VERTICES that exist in the graph. The graph's adjacency matrix will NOT
// be freed.
/**
 * @brief Clear the connections between __G's vertices
 *
 * Free all of the nodes that are contained in __G's adjacency list. This function does *NOT*
 * free the actual array pointed to by __G->adj, only the connections that currently exist.
 *
 * @param __G The graph whose connections will be wiped
 */
void cleanGraph(Graph *__G);

// Free all of the nodes of a graph and also free the adjacency matrix. This still does not
// free the actual pointer __G. To do so, see releaseGraph
/**
 * @brief Clean the nodes of a graph and free it's adjacency list array.
 *
 * This routine still doesn't free the actual memory that the pointer __G points to.
 * @see releaseGraph
 *
 * @param __G
 */
void freeGraph(Graph *__G);

/**
 * @brief call freeGraph(*__Gptr), free *__Gptr, and set *__Gptr to NULL.
 *
 * This is a routine used to completely free all of the allocated memory associated with the graph *__Gptr.
 * This routine should be called when a program is no longer in use of the Graph pointed to by *__Gptr.
 *
 * @param __Gptr
 */
void releaseGraph(Graph **__Gptr);

/**
 * @brief Print the connections stored in the adjacency list of a graph.
 *
 * @param __g
 */
void printGraph(const Graph *__g);

/**
 * @brief
 * @private
 * @deprecated
 *
 * @param __g
 * @param __v1
 * @param __v2
 * @return int
 */
int _add_vertex(Graph *__g, int __v1, int __v2);

// Return the position where this was added
/**
 * @brief Create a new connection between two vertices in a graph
 *
 * @param __g  The graph in which the new connection will be made
 * @param __v1 The first vertex
 * @param __v2 The second vertex
 * @return int
 */
int addEdge(Graph *__g, int __v1, int __v2);

// Scan from a stdinput
Graph *readGraph(const char *__filename, bool __digraph);

// Initialize an array of booleans to false to check if they've been visited or not
bool *visitedArray(const Graph *__g);

// Create a dot output of the loaded graph to be visualized with graphviz
int createDot(const Graph *g, const char *__filename);

// Wrapper routine to access the adjacency list of vertex v
Vertex graphAdj(const Graph *__g, Vertex __v);

// __v is the starting vertex
void dfsVisualize(const Graph *__g, GRAPH_TYPE __v);

// This routine has a bool array that is passed down between successive calls.
/**
 * @brief
 * @private
 *
 * @param __g
 * @param __v
 * @param __visited
 */
void _dfsVisualize(const Graph *__g, int __v, bool *__visited);

// Return true iff the two elements are connected
bool dfsConnected(const Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __u);

/**
 * @brief
 * @private
 *
 * @param __g
 * @param __v
 * @param __u
 * @param __visited
 */
void dfsConnected_(const Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __u, bool *__visited);

void bfsVisualize(const Graph *__g, int __v);

// Take in the extra bool array visited
/**
 * @brief
 * @private
 *
 * @param __g
 * @param __v
 * @param __visited
 * @param __q
 */
void bfsVisualize_(const Graph *__g, int __v, bool *__visited, Queue *__q);

// Using DFS, extract all the elements that are connected to v
Graph *subgraph(const Graph *__g, int __v);

/**
 * @brief
 * @private
 *
 * @param __og
 * @param __gnew
 * @param __start
 * @param __v
 * @param __visited
 */
void subgraph_(const Graph *__og, Graph *__gnew, int __start, int __v, bool *__visited);

void createDotSubgraph(const Graph *g, const char *__filename);

// Return true if every node can be reached starting from node 1 and performing a
// Depth first search.
bool isConnected(const Graph *g);

// DFS search to see if the graph is connected
/**
 * @brief
 * @private
 *
 * @param __g
 * @param __v
 * @param __visited
 * @param __count
 */
void isConnected_(const Graph *__g, int __v, bool *__visited, int *__count);

// Take a directional graph and reverse the direction of its edges.
// This function allocates the space for a new graph.
Graph *reverseGraph(const Graph *__G);

/**
 * @brief
 * @private
 *
 * @param __G
 * @param __dup
 * @param __node
 * @param __visited
 * @return void*
 */
void *reverseGraph_(const Graph *__G, Graph *__dup, int __node, bool *__visited);

// Implement Kosaraju's algorithm to find strongly connected components.
// The obvious return type is a graph whose only elements are the strongly connected ones...
Graph *stronglyConnected(const Graph *__g);

/**
 * @brief
 * @private
 *
 * @param __g
 * @param __v
 * @param __stack
 * @param __visited
 */
void stronglyConnected_(const Graph *__g, int __v, Stack *__stack, bool *__visited);

/**
 *
 * @brief
 * @private
 *
 * @param __g
 * @param __gnew
 * @param __v
 * @param __stack
 * @param __visited
 * @param __scc
 */
void stronglyConnectedRev_(const Graph *__g, Graph *__gnew, int __v, Stack *__stack, bool *__visited, bool *__scc);

/*========================================================================
 *!                          Utility functions
 *========================================================================*/

// I want a function that get's a non null vertex from a Graph.
// If the Graph is empty (i.e.) there are no connections
// get the INDEX of the position of a vertex
// returns -1 if there are no connections
int getVertex(const Graph *__G);


// __v is the starting vertex
bool *traverse(const Graph *__g, GRAPH_TYPE __v);

// This routine has a bool array that is passed down between successive calls.
/**
 * @brief
 * @private
 *
 * @param __g
 * @param __v
 * @param __visited
 */
void traverse_(const Graph *__g, int __v, bool *__visited);

bool isStronglyConnected(const Graph *__rev, int __v, bool *__visited, bool *__scc);

Graph *removeEdge(const Graph *__g, int __v1, int __v2);

void removeEdge_(Graph *__g, int __v1, int __v2);

// Return true if __v1 is ADJACENT to __v2
bool adjacentTo(const Graph *__g, int __v1, int __v2);

bool isBridge(const Graph *__g, int __v1, int __v2);

// Return true if the any of the edges of a graph are bridges.
// This is accomplished by performing a depth first search starting from
// the first available node.
bool hasBridge(const Graph *__g);

// simple dfs implementation
bool hasBridge_(const Graph *__g, int __v, bool *__visited);

// On a non-oriented graph, remove the reciprocal connections that are only used for
// storing purposes. For example, if 1 and 3 are connected, remove the connection 3 -- 1
Graph *removeMirrorConnection(const Graph *__g);

void removeMirrorConnection_(Graph *__g, int __v, bool *__visited);

void reverseEdge(Graph *__g, int __v1, int __v2);
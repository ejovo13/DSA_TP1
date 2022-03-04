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
 * @param nv Number of vertices
 * @param ne Number of edges
 * @param digraph True if the graph is oriented
 * @return Graph* A pointer to the newly allocated Graph structure
 */
Graph *newGraph(int nv, int ne, bool digraph);

// replicate the connections of graph G and return a newly allocated graph
/**
 * @brief Perform a deep copy of a graph
 *
 * Create a new graph whose connections are exactly the same as G, but whose adjacency list
 * is full of newly allocated nodes. This way the new graph has complete control of the elements that
 * it points to
 *
 * @param G The Graph to duplicate
 * @return A pointer to the newly allocated copy
 */
Graph *duplicateGraph(const Graph *G);

// Free all of the VERTICES that exist in the graph. The graph's adjacency matrix will NOT
// be freed.
/**
 * @brief Clear the connections between G's vertices
 *
 * Free all of the nodes that are contained in G's adjacency list. This function does *NOT*
 * free the actual array pointed to by G->adj, only the connections that currently exist.
 *
 * @param G The graph whose connections will be wiped
 */
void cleanGraph(Graph *G);

// Free all of the nodes of a graph and also free the adjacency matrix. This still does not
// free the actual pointer G. To do so, see releaseGraph
/**
 * @brief Clean the nodes of a graph and free it's adjacency list array.
 *
 * This routine still doesn't free the actual memory that the pointer G points to.
 * @see releaseGraph
 *
 * @param G
 */
void freeGraph(Graph *G);

/**
 * @brief call freeGraph(*Gptr), free *Gptr, and set *Gptr to NULL.
 *
 * This is a routine used to completely free all of the allocated memory associated with the graph *Gptr.
 * This routine should be called when a program is no longer in use of the Graph pointed to by *Gptr.
 *
 * @param Gptr
 */
void releaseGraph(Graph **Gptr);

/**
 * @brief Print the connections stored in the adjacency list of a graph.
 *
 * @param g
 */
void printGraph(const Graph *g);

/**
 * @brief
 * @private
 * @deprecated
 *
 * @param g
 * @param v1
 * @param v2
 * @return int
 */
int _add_vertex(Graph *g, int v1, int v2);

// Return the position where this was added
/**
 * @brief Create a new connection between two vertices in a graph
 *
 * @param g  The graph in which the new connection will be made
 * @param v1 The first vertex
 * @param v2 The second vertex
 * @return int
 */
int addEdge(Graph *g, int v1, int v2);

// Scan from a stdinput
Graph *readGraph(const char *filename, bool digraph);

// Initialize an array of booleans to false to check if they've been visited or not
bool *visitedArray(const Graph *g);

// Create a dot output of the loaded graph to be visualized with graphviz
int createDot(const Graph *g, const char *filename);

// Wrapper routine to access the adjacency list of vertex v
Vertex graphAdj(const Graph *g, Vertex v);

// v is the starting vertex
void dfsVisualize(const Graph *g, GRAPH_TYPE v);

// This routine has a bool array that is passed down between successive calls.
/**
 * @brief
 * @private
 *
 * @param g
 * @param v
 * @param visited
 */
void _dfsVisualize(const Graph *g, int v, bool *visited);

// Return true iff the two elements are connected
bool dfsConnected(const Graph *g, GRAPH_TYPE v, GRAPH_TYPE u);

/**
 * @brief
 * @private
 *
 * @param g
 * @param v
 * @param u
 * @param visited
 */
void dfsConnected_(const Graph *g, GRAPH_TYPE v, GRAPH_TYPE u, bool *visited);

void bfsVisualize(const Graph *g, int v);

// Take in the extra bool array visited
/**
 * @brief
 * @private
 *
 * @param g
 * @param v
 * @param visited
 * @param q
 */
void bfsVisualize_(const Graph *g, int v, bool *visited, Queue *q);

// Using DFS, extract all the elements that are connected to v
Graph *subgraph(const Graph *g, int v);

/**
 * @brief
 * @private
 *
 * @param og
 * @param gnew
 * @param start
 * @param v
 * @param visited
 */
void subgraph_(const Graph *og, Graph *gnew, int start, int v, bool *visited);

void createDotSubgraph(const Graph *g, const char *filename);

// Return true if every node can be reached starting from node 1 and performing a
// Depth first search.
bool isConnected(const Graph *g);

// DFS search to see if the graph is connected
/**
 * @brief
 * @private
 *
 * @param g
 * @param v
 * @param visited
 * @param count
 */
void isConnected_(const Graph *g, int v, bool *visited, int *count);

// Take a directional graph and reverse the direction of its edges.
// This function allocates the space for a new graph.
Graph *reverseGraph(const Graph *G);

/**
 * @brief
 * @private
 *
 * @param G
 * @param dup
 * @param node
 * @param visited
 * @return void*
 */
void *reverseGraph_(const Graph *G, Graph *dup, int node, bool *visited);

// Implement Kosaraju's algorithm to find strongly connected components.
// The obvious return type is a graph whose only elements are the strongly connected ones...
Graph *stronglyConnected(const Graph *g);

/**
 * @brief
 * @private
 *
 * @param g
 * @param v
 * @param stack
 * @param visited
 */
void stronglyConnected_(const Graph *g, int v, Stack *stack, bool *visited);

/**
 *
 * @brief
 * @private
 *
 * @param g
 * @param gnew
 * @param v
 * @param stack
 * @param visited
 * @param scc
 */
void stronglyConnectedRev_(const Graph *g, Graph *gnew, int v, Stack *stack, bool *visited, bool *scc);

/*========================================================================
 *!                          Utility functions
 *========================================================================*/

// I want a function that get's a non null vertex from a Graph.
// If the Graph is empty (i.e.) there are no connections
// get the INDEX of the position of a vertex
// returns -1 if there are no connections
int getVertex(const Graph *G);


// v is the starting vertex
bool *traverse(const Graph *g, GRAPH_TYPE v);

// This routine has a bool array that is passed down between successive calls.
/**
 * @brief
 * @private
 *
 * @param g
 * @param v
 * @param visited
 */
void traverse_(const Graph *g, int v, bool *visited);

bool isStronglyConnected(const Graph *rev, int v, bool *visited, bool *scc);

Graph *removeEdge(const Graph *g, int v1, int v2);

void removeEdge_(Graph *g, int v1, int v2);

// Return true if v1 is ADJACENT to v2
bool adjacentTo(const Graph *g, int v1, int v2);

bool isBridge(const Graph *g, int v1, int v2);

// Return true if the any of the edges of a graph are bridges.
// This is accomplished by performing a depth first search starting from
// the first available node.
bool hasBridge(const Graph *g);

// simple dfs implementation
bool hasBridge_(const Graph *g, int v, bool *visited);

// On a non-oriented graph, remove the reciprocal connections that are only used for
// storing purposes. For example, if 1 and 3 are connected, remove the connection 3 -- 1
Graph *removeMirrorConnection(const Graph *g);

void removeMirrorConnection_(Graph *g, int v, bool *visited);

void reverseEdge(Graph *g, int v1, int v2);
#pragma once

#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "queue.h"
#include "stack.h"

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

Graph *newGraph(int __nv, int __ne, bool __digraph);

// replicate the connections of graph __G and return a newly allocated graph
Graph *duplicateGraph(Graph *__G);

void printGraph(Graph *__g);

int _add_vertex(Graph *__g, int __v1, int __v2);

// Return the position where this was added
int addVertex(Graph *__g, int __v1, int __v2);

// Scan from a stdinput
Graph *readGraph(const char *__filename, bool __digraph);

// Initialize an array of booleans to false to check if they've been visited or not
bool *visitedArray(Graph *__g);

// Create a dot output of the loaded graph to be visualized with graphviz
int createDot(Graph *g, const char *__filename);

// Wrapper routine to access the adjacency list of vertex v
Vertex graphAdj(Graph *__g, Vertex __v);

// __v is the starting vertex
void dfsVisualize(Graph *__g, GRAPH_TYPE __v);

// This routine has a bool array that is passed down between successive calls.
void _dfsVisualize(Graph *__g, int __v, bool *__visited);

// Return true iff the two elements are connected
bool dfsConnected(Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __u);

void dfsConnected_(Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __u, bool *__visited);

void bfsVisualize(Graph *__g, int __v);

// Take in the extra bool array visited
void bfsVisualize_(Graph *__g, int __v, bool *__visited, Queue *__q);

// Using DFS, extract all the elements that are connected to v
Graph *subgraph(Graph *__g, int __v);

void subgraph_(Graph *__og, Graph *__gnew, int __start, int __v, bool *__visited);

void createDotSubgraph(Graph *g, const char *__filename);

// Return true if every node can be reached starting from node 1 and performing a
// Depth first search.
bool isConnected(Graph *g);

// DFS search to see if the graph is connected
void isConnected_(Graph *__g, int __v, bool *__visited, int *__count);

// Take a directional graph and reverse the direction of its edges.
// This function allocates the space for a new graph.
Graph *reverseGraph(Graph *__G);

// Here I'm traversing __G while adding to __dup.
void *reverseGraph_(Graph *__G, Graph *__dup, int __node, bool *__visited);
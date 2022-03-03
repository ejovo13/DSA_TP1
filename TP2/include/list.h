// Implement a simple linked list that contains vertices.
// The List will be implemented as a simple "Node" structure

//
// Node -> Node -> Node
//

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define GRAPH_TYPE int

/**
 * @brief Implement a Linked List data type that will be used in the adjacency list of the Graph
 * @file
 *
 */

typedef struct node_t {

    GRAPH_TYPE data;
    struct node_t *next;

} Node;

enum VERTEX_STATUS { NEW_VERTEX, OLD_VERTEX };

// To facility reading, here I introduce two type synonyms
typedef Node** AdjacencyList;
typedef Node* Vertex;

Node *newNode(GRAPH_TYPE val);

void freeNode(Node *__n);

void freeList(Node *__n);

// Used to duplicate a list of vertices in a graph's adjacency list
Node *duplicateList(Node *__n);

void printVertices(Vertex __vs);

// Count how many vertices come after this one (and including it)
int countVertex(Vertex __v);

// Count how many unvisited vertices are adjacent to this one, without modifying the
// visited array.
int countUnvisitedVertex(Vertex __v, bool *__visited);

// Write a vertex to a file
void writeVertex(Vertex __v, FILE *__f, bool __di, bool *__visited);

// Wrapper routine to check whether or not a vertex has already been visited.
// It's really fing annoying that the vertices are read in starting at 1 and not 0.
// Maybe I'll change that later so that the vertices are internally stored starting
// at 0 and only when I print to the screen will I increment the value
bool visited(Vertex __v, bool *__visited);

// Write a vertex to a file
void writeVertexSubgraph(int __root, Vertex __v, FILE *__f, bool __di, bool *__visited);
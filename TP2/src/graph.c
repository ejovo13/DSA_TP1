#include "graph.h"
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

Graph *newGraph(int __nv, int __ne, bool __digraph) {

    // First thing to do is allocate the space for a new adjacency list matrix
    Graph *g = (Graph *) malloc(sizeof(Graph));
    // Now initialize the adjacenty list with elements 0 ... n-1
    // Allocate space for the underlying matrix
    if (g) {
        g->adj = (Vertex *) malloc(sizeof(Vertex) * __nv);
    }

    g->nvert = __nv;
    // g->nedge = __ne;
    g->nedge = 0;
    g->di = __digraph;

    for (int i = 0; i < __nv; i++) {
        // g->adj[i] = newNode(i);
        g->adj[i] = NULL;
    }

    return g;
}

// replicate the connections of graph __G and return a newly allocated graph
Graph *duplicateGraph(const Graph *__G) {

    Graph *copy_of_g = newGraph(__G->nvert, __G->nedge, __G->di); // initialize the adjacency list

    // Iterate through each node and duplicate the list of vertices
    for (int i = 0; i < __G->nvert; i++) {
        copy_of_g->adj[i] = duplicateList(__G->adj[i]);
    }

    return copy_of_g;
}

// Free all of the VERTICES that exist in the graph. The graph's adjacency matrix will NOT
// be freed.
void cleanGraph(Graph *__G) {

    if (!__G) return;

    for (int i = 0; i < __G->nedge; i++) {
        freeList(__G->adj[i]);
    }

    __G->nedge = 0;
}

// Free all of the nodes of a graph and also free the adjacency matrix. This still does not
// free the actual pointer __G. To do so, see releaseGraph
void freeGraph(Graph *__G) {

    cleanGraph(__G);
    if (__G->adj) free(__G->adj);

}

void releaseGraph(Graph **__Gptr) {

    if (!__Gptr) return;

    freeGraph(*__Gptr);
    *__Gptr = NULL;
}


void printGraph(const Graph *__g) {

    printf("N edges: %d, N vertices: %d\n", __g->nedge, __g->nvert);

    for (int i = 0; i < __g->nvert; i++) {
        printf("%d -> ", i + 1);
        printVertices(__g->adj[i]);
    }
}

int _add_vertex(Graph *__g, int __v1, int __v2) {

    __g->nedge ++;

    Vertex v1 = __g->adj[__v1 - 1];

    // printf("Joining %d and %d\n", __v1, __v2);

    // If the left vertex has no connections
    if (!v1) {
        __g->adj[__v1 - 1] = newNode(__v2); // immediately add it
    }

    Vertex it = v1;

    while(it) {

        if(it->data == __v2) return OLD_VERTEX; // then we already have these two elements joined
        else if (!it->next) {
            it->next = newNode(__v2); // If we are at the end, add it
            return NEW_VERTEX;
        }
        // count ++;
        it = it->next;
    }
}

// Return the position where this was added
int addVertex(Graph *__g, int __v1, int __v2) {

    int stat = _add_vertex(__g, __v1, __v2);
    if (!__g->di) _add_vertex(__g, __v2, __v1);
}

// Scan from a stdinput
Graph *readGraph(const char *__filename, bool __digraph) {

    FILE *f = fopen(__filename, "r");
    int nvert = 0;
    int nedge = 0;
    int vert1 = 0; // Left hand vertex when reading the file
    int vert2 = 0; // Right hand vertex when reading in the graph

    char line[256]; // buffer for a line

    // get the number of vertices;
    if (fgets(line, sizeof(line), f)) sscanf(line, "%d", &nvert);
    if (fgets(line, sizeof(line), f)) sscanf(line, "%d", &nedge);

    // printf("Creating a new")
    Graph *g = newGraph(nvert, nedge, __digraph);

    while (fgets(line, sizeof(line), f)) {

        sscanf(line, "%d %d", &vert1, &vert2);
        addVertex(g, vert1, vert2);
    }

    fclose(f);

    return g;
}

// Initialize an array of booleans to false to check if they've been visited or not
bool *visitedArray(const Graph *__g) {

    bool *visited = (bool *) calloc(sizeof(bool), __g->nvert);
    return visited;

}

// Create a dot output of the loaded graph to be visualized with graphviz
int createDot(const Graph *g, const char *__filename) {

    // first open the file to write
    FILE *f = fopen(__filename, "w");

    if (g->di) fprintf(f, "di");
    fprintf(f, "graph {\n");

    bool *__visited = visitedArray(g);

    for (int i = 0; i < g->nvert; i++) {
        fprintf(f, "  %d", i + 1);
        writeVertex(g->adj[i], f, g->di, __visited);
    }

    fprintf(f, "}\n");
    fclose(f);

    return 0;
}

// Wrapper routine to access the adjacency list of vertex v
Vertex graphAdj(const Graph *__g, Vertex __v) {
    return __g->adj[__v->data - 1];
}

// __v is the starting vertex
void dfsVisualize(const Graph *__g, GRAPH_TYPE __v) {
    bool *visited = visitedArray(__g);
    _dfsVisualize(__g, __v, visited);
    free(visited);
}

// This routine has a bool array that is passed down between successive calls.
void _dfsVisualize(const Graph *__g, int __v, bool *__visited) {

    // Print that we've visited this node
    printf("Visiting node %d\n", __v);
    __visited[__v - 1] = true; // Mark as visited

    Vertex it = __g->adj[__v - 1];
    // Iterate through all of the children
    while (it) {
        if (!visited(it, __visited)) _dfsVisualize(__g, it->data, __visited);
        it = it->next;
    }
}

// Return true iff the two elements are connected
bool dfsConnected(const Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __u) {

    bool *visited = visitedArray(__g);
    dfsConnected_(__g, __v, __u, visited);

    bool areConnected = visited[__u - 1];
    free(visited);

    return areConnected;
}

void dfsConnected_(const Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __u, bool *__visited) {

    // bool *visited = visitedArray(__g);
    // printf("Visiting node %d\n", __v);
    __visited[__v - 1] = true;
    if (__v == __u) return;

    Vertex it = __g->adj[__v - 1];

    while (it) {
        if (!visited(it, __visited)) dfsConnected_(__g, it->data, __u, __visited);
        it = it->next;
    }
}

void bfsVisualize(const Graph *__g, int __v) {

    bool *visited = visitedArray(__g);
    Queue *q = newQueue();
    bfsVisualize_(__g, __v, visited, q);
    // TODO Free the queue!?
    free(visited);
}

// Take in the extra bool array visited
void bfsVisualize_(const Graph *__g, int __v, bool *__visited, Queue *__q) {

    // Mark as visited
    __visited[__v - 1] = true;
    printf("Visiting %d\n", __v);
    Vertex it = __g->adj[__v - 1];

    // Add all of the children to the queue
    while (it) { // While there are children,
        enQueue(__q, it->data); // add them to the queue.
        it = it->next;
    }

    // Then iterate throgh the queue!
    Vertex v_search = deQueue(__q);
    while (v_search) {
        if (!visited(v_search, __visited)) bfsVisualize_(__g, v_search->data, __visited, __q);
        v_search = deQueue(__q);
    }
}

// Using DFS, extract all the elements that are connected to v
Graph *subgraph(const Graph *__g, int __v) {

    // First create a new graph with the same dimensions
    Graph *g = newGraph(__g->nvert, __g->nedge, __g->di);
    bool *visited = visitedArray(__g);
    visited[__v - 1] = true;
    // Now perform DFS, adding connect
    subgraph_(__g, g, __v, __v, visited);

    free(visited);
    return g;
}

void subgraph_(const Graph *__og, Graph *__gnew, int __start, int __v, bool *__visited) {

    if (!__visited[__v - 1]) addVertex(__gnew, __start, __v);
    __visited[__v - 1] = true;
    Vertex it = __og->adj[__v - 1];


    // Iterate through all of the children
    while (it) {
        if (!visited(it, __visited)) subgraph_(__og, __gnew, __start, it->data, __visited);
        it = it->next;
    }
}

void createDotSubgraph(const Graph *g, const char *__filename) {
// Create a dot output of the loaded graph to be visualized with graphviz
// A subgraph will not include nodes that have no connections

    // first open the file to write
    FILE *f = fopen(__filename, "w");

    if (g->di) fprintf(f, "di");
    fprintf(f, "graph {\n");

    bool *__visited = visitedArray(g);

    for (int i = 0; i < g->nvert; i++) {
        // fprintf(f, "  %d", i + 1);// We only write this if there is a non null....
        writeVertexSubgraph(i + 1, g->adj[i], f, g->di, __visited);
    }

    fprintf(f, "}\n");
    fclose(f);

    // return 0;
}

// Return true if every node can be reached starting from node 1 and performing a
// Depth first search.
bool isConnected(const Graph *g) {

    int count = 0;
    bool *visited = visitedArray(g);
    int startNode = 1;

    // Count the connections
    isConnected_(g, startNode, visited, &count);

    return count == g->nvert;
}

// DFS search to see if the graph is connected
void isConnected_(const Graph *__g, int __v, bool *__visited, int *__count) {

    __visited[__v - 1] = true;
    (*__count) ++;

    Vertex it = __g->adj[__v - 1];
    while (it) {
        if (!visited(it, __visited)) isConnected_(__g, it->data, __visited, __count);
        it = it->next;
    }

}

// Take a directional graph and reverse the direction of its edges.
// This function allocates the space for a new graph.
Graph *reverseGraph(const Graph *__G) {

    if (!__G->di) return duplicateGraph(__G);

    // Otherwise, let's do a depth first search to visit every node.

    // Every time that we visit a node we want to take the previous node and make a new connection,
    // albeit in the reverse direction

    Graph *rev = newGraph(__G->nvert, __G->nedge, true);

    bool *visited = visitedArray(__G);

    // by default start at the first node
    reverseGraph_(__G, rev, 1, visited);
    free(visited);

    return rev;
}

// Here I'm traversing __G while adding to __dup.
void *reverseGraph_(const Graph *__G, Graph *__dup, int __node, bool *__visited) {

    __visited[__node - 1] = true;

    Vertex it = __G->adj[__node - 1];

    while (it) {

        addVertex(__dup, it->data, __node);
        if (!visited(it, __visited)) reverseGraph_(__G, __dup, it->data, __visited);
        it = it->next;
    }
}

// Implement Kosaraju's algorithm to find strongly connected components.
// The obvious return type is a graph whose only elements are the strongly connected ones...
Graph *stronglyConnected(const Graph *__g) {

    bool *visited = visitedArray(__g);
    Stack *stack = newStack();

    int ind = getVertex(__g);
    // start searching from a confirmed position

    Graph *gnew = newGraph(__g->nvert, 0, __g->di);

    stronglyConnected_(__g, gnew, ind + 1, stack, visited);

    printf("Found strongly connected components\n");
    printStack(stack);


    return gnew;

}

void stronglyConnected_(const Graph *__g, Graph *__gnew, int __v, Stack *__stack, bool *__visited) {

    printf("Visiting %d\n", __v);
    __visited[__v - 1] = true;

    Vertex it = __g->adj[__v - 1];
    // printf("it: %d\n", it->data);

    while (it) {
        // if (!visited(it, __visited)) dfsConnected_(__g, it->data, __u, __visited);
        if (visited(it, __visited)) {
            pushStack(__stack, __v);
            printf("Pushing to stack!\n");
        }

        if (!graphAdj(__g, it)) {
            pushStack(__stack, it->data);
            printf("Pushing end point to stack!\n");
        }

        if (!visited(it, __visited)) stronglyConnected_(__g, __gnew, it->data, __stack, __visited);

        it = it->next;
    }


}

/**========================================================================
 *!                           Utility functions
 *========================================================================**/

// I want a function that get's a non null vertex from a Graph.
// If the Graph is empty (i.e.) there are no connections
int getVertex(const Graph *__G) {

    for (int i = 0; i < __G->nvert; i++) {
        if (__G->adj[i]) return i; // If the pointer is non null (i.e. there is a connection)
    }

    return -1;
}
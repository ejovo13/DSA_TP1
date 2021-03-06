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

/**
 *
 * @brief Hello hello
 *
 * @param __nv
 * @param __ne
 * @param __digraph
 * @return * Graph*
 */

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
int addEdge(Graph *__g, int __v1, int __v2) {

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
        addEdge(g, vert1, vert2);
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
int createDot(const Graph *__g, const char *__filename) {

    // first open the file to write
    FILE *f = fopen(__filename, "w");
    Graph *g = duplicateGraph(__g);

    if (g->di) fprintf(f, "di");
    else g = removeMirrorConnection(__g);
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

    if (!__visited[__v - 1]) addEdge(__gnew, __start, __v);
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

        addEdge(__dup, it->data, __node);
        if (!visited(it, __visited)) reverseGraph_(__G, __dup, it->data, __visited);
        it = it->next;
    }
}

// Implement Kosaraju's algorithm to find strongly connected components.
// The obvious return type is a graph whose only elements are the strongly connected ones...
Graph *stronglyConnected(const Graph *__g) {

    bool *vis = visitedArray(__g);
    bool *scc = visitedArray(__g);
    Stack *stack = newStack();

    int ind = getVertex(__g);
    // start searching from a confirmed position

    Graph *gnew = newGraph(__g->nvert, 0, __g->di);

    stronglyConnected_(__g, ind + 1, stack, vis);
    // Add the end points and the the precedent of already visited nodes to
    // the stack

    Graph *grev = reverseGraph(__g);

    // Nw we have to start from the top vertex of the stack,
    // traverse its child vertices.

    // Now let's iterate through the stack, adding edges between unvisited vertices

    free(vis);
    vis = visitedArray(__g);

    Vertex it = popStack(stack);

    while (it) {

        if (!visited(it, vis)) {

            // If the node in our stack has 0 neighbors, add it to the scc and move on.
            if (!graphAdj(__g, it)) {

                scc[it->data - 1] = true;
                free(it);
                it = popStack(stack);
                continue;

            }

            // _dfsVisualize(gnew, it->data, vis);
            stronglyConnectedRev_(grev, gnew, it->data, stack, vis, scc);
            traverse_(gnew, it->data, scc); // Mark all the components connected to it as being part of
                                            // a strongly connected component
        }

        free(it);
        it = popStack(stack);

    }

    free(vis);
    free(scc);

    return gnew;
}

void stronglyConnected_(const Graph *__g, int __v, Stack *__stack, bool *__visited) {

    printf("Visiting %d\n", __v);
    __visited[__v - 1] = true;

    Vertex it = __g->adj[__v - 1];
    // printf("it: %d\n", it->data);

    while (it) {
        // if (!visited(it, __visited)) dfsConnected_(__g, it->data, __u, __visited);
        if (visited(it, __visited)) {
            pushStack(__stack, __v);
        }

        if (!graphAdj(__g, it)) {
            pushStack(__stack, it->data);
            return;
        }

        if (!visited(it, __visited)) stronglyConnected_(__g,it->data, __stack, __visited);

        it = it->next;
    }

    // outside of the depth first search, start going in reverse.
    // If all of the children of this node are visited, then add it to the stack

    it = __g->adj[__v - 1];

    while (it) {
        if (!visited(it, __visited)) return;
        it = it->next;
    }

    pushStack(__stack, __v);

}

void stronglyConnectedRev_(const Graph *__g, Graph *__gnew, int __v, Stack *__stack, bool *__visited, bool *__scc) {

    __visited[__v - 1] = true;

    Vertex it = __g->adj[__v - 1];

    while (it) {

        if (visited(it, __scc)) {
            it = it->next;
            continue;
        };

        // Here the problem is that no matter what I do, I add an Edge. What I need to consider is the case
        // where I end up at a leaf without having found a strongly connected component. So what I really need
        // to do is only end up adding a set to the list if I've actually found a strongly connected component.
        addEdge(__gnew, it->data, __v);

        if (visited(it, __visited)) return;

        stronglyConnectedRev_(__g, __gnew, it->data, __stack, __visited, __scc);

        it = it->next;

    }

}

// mark strongly connected components

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


// __v is the starting vertex
bool *traverse(const Graph *__g, GRAPH_TYPE __v) {
    bool *visited = visitedArray(__g);
    _dfsVisualize(__g, __v, visited);
    return visited;
}

// This routine has a bool array that is passed down between successive calls.
void traverse_(const Graph *__g, int __v, bool *__visited) {

    // Print that we've visited this node
    printf("<=== Trv ===> MARKING %d\n", __v);
    __visited[__v - 1] = true; // Mark as visited

    Vertex it = __g->adj[__v - 1];
    // Iterate through all of the children
    while (it) {
        if (!visited(it, __visited)) traverse_(__g, it->data, __visited);
        it = it->next;
    }
}


bool isStronglyConnected(const Graph *__rev, int __v, bool *__visited, bool *__scc) {

    // traverse by dfs
    __visited[__v - 1] = true;
    Vertex it = __rev->adj[__v - 1];

    while (it) {

        if (visited(it, __visited) && !visited(it, __scc)) {
            printf("Stopping at visited component (but not in scc) !!! left from %d, exiting at %d\n", 0, __v);
            return true;
        }

        if (visited(it, __visited)) {
            it = it->next;
            continue;
        }


        return isStronglyConnected(__rev, it->data, __visited, __scc);
        it = it->next;
    }

    return false;

}


// Need to implement a new procedure here that Actually exports the strongly connected components to a new
// dot file, adding color to unique components/somehow indicate that all of the vertices of a certain cluster
// are strongly connected.

//=========================== PARTIE 3 ============================

// First procedure that I want to implement is a way to delete a connection from a graph.

Graph *removeEdge(const Graph *__g, int __v1, int __v2) {

    // Duplicate the graph and remove the edge (if it exists);
    Graph *dup = duplicateGraph(__g);

    // The first thing that I want to do is see if the connection even exists...
    if (adjacentTo(__g, __v1, __v2)) {

        removeEdge_(dup, __v1, __v2);
        if (__g->di) removeEdge_(dup, __v2, __v1);


    }

    return dup;
}

void removeEdge_(Graph *__g, int __v1, int __v2) {

    Vertex it = __g->adj[__v1 - 1];
    Vertex prev = it;

    // If the connection is the top of the adjacency list...
    if (prev) {

        if (prev->data == __v2) {

            printf("Removing first connection..\n");
            __g->adj[__v1 - 1] = it->next;
            // free(prev);
            return;
        }
    }

    while (it) {

        if (it->data == __v2) {
            prev->next = it->next;
            free(it);
            return;
        }

        prev = it;
        it = it->next;
    }
}

// Return true if __v1 is ADJACENT to __v2
bool adjacentTo(const Graph *__g, int __v1, int __v2) {

    // Iterate through the neighbors of __v1 and see if __v2 is contained.

    Vertex it = __g->adj[__v1 - 1];

    while (it) {

        if (it->data == __v2) return true;
        it = it->next;
    }

    return false;

}


// Return true if the highlighted edge is a bridge
// An edge is a bridge if removing the edge leaves us with a non connected graph.
bool isBridge(const Graph *__g, int __v1, int __v2) {

    Graph *g_removed = removeEdge(__g, __v1, __v2);

    bool out = isConnected(g_removed);

    releaseGraph(&g_removed); // Take care of memory allocation for the graph.

    return out;
}

// Return true if the any of the edges of a graph are bridges.
// This is accomplished by performing a depth first search starting from
// the first available node.
bool hasBridge(const Graph *__g) {

    bool *vis = visitedArray(__g);

    int v = getVertex(__g);

    if (v == -1) return false; // This graph has no connections

    printf("Entering has bridge with v = %d\n", v);

    return hasBridge_(__g, v + 1, vis);

}

// simple dfs implementation
bool hasBridge_(const Graph *__g, int __v, bool *__visited) {

    __visited[__v - 1] = true;

    Vertex it = __g->adj[__v - 1];

    while (it) {

        if (isBridge(__g, __v, it->data)) {
            printf("Bridge detected between %d-%d\n", __v, it->data);
            return true;
        }
        if (!visited(it, __visited)) return hasBridge_(__g, it->data, __visited);

        it = it->next;

    }

    return false;

}

// On a non-oriented graph, remove the reciprocal connections that are only used for
// storing purposes. For example, if 1 and 3 are connected, remove the connection 3 -- 1
Graph *removeMirrorConnection(const Graph *__g) {

    if (__g->di) return duplicateGraph(__g);

    // perform dfs on all of the nodes, removing the reverse as we arrive
    Graph *g_dup = duplicateGraph(__g);
    removeMirrorConnection_(g_dup, getVertex(g_dup) + 1, visitedArray(g_dup));

    return g_dup;

}

void removeMirrorConnection_(Graph *__g, int __v, bool *__visited) {

    __visited[__v - 1] = true;

    Vertex it = __g->adj[__v - 1];

    while (it) {

        removeEdge_(__g, it->data, __v);
        if (!visited(it, __visited)) removeMirrorConnection_(__g, it->data, __visited);

        it = it->next;
    }

}

// Simply change the connection from (1 -> 3) to (3 -> 1)
void reverseEdge(Graph *__g, int __v1, int __v2) {


    if (adjacentTo(__g, __v1, __v2)) {
        removeEdge_(__g, __v1, __v2);
        addEdge(__g, __v2, __v1);
    }

}
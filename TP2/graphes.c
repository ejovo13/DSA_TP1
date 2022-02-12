#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
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

#define GRAPH_TYPE int

typedef struct node_t {

    GRAPH_TYPE data;
    struct node_t *next;

} Node;

// Is this structure really necessary?
typedef struct {

    Node *head;

} List;

// typedef struct {

//     // Here we want an array of length V whose components are LISTS
//     Node** adj_list;

// } Graph;


typedef Node** AdjacencyList;
typedef Node* Vertex;

typedef struct {

    AdjacencyList adj;
    int nedge;
    int nvert;
    bool di;

} Graph;


Node *newNode(GRAPH_TYPE val) {

    Node *n = (Node *) malloc(sizeof(Node));
    n->data = val;
    return n;
}

Graph *newGraph(int __nv, int __ne, bool __digraph) {

    // First thing to do is allocate the space for a new adjacency list matrix
    Graph *g = (Graph *) malloc(sizeof(Graph));
    // Now initialize the adjacenty list with elements 0 ... n-1
    // Allocate space for the underlying matrix
    if (g) {
        g->adj = (Vertex *) malloc(sizeof(Vertex) * __nv);
    }

    g->nvert = __nv;
    g->nedge = __ne;
    g->di = __digraph;

    for (int i = 0; i < __nv; i++) {
        // g->adj[i] = newNode(i);
        g->adj[i] = NULL;
    }

    return g;
}

void printVertices(Vertex __vs) {

    Vertex it = __vs;
    while (it) {
        printf("%d ", it->data);
        it = it->next;
    }
    printf("\n");

}

void printGraph(Graph *__g) {

    printf("N edges: %d, N vertices: %d\n", __g->nedge, __g->nvert);

    for (int i = 0; i < __g->nvert; i++) {
        printf("%d -> ", i + 1);
        printVertices(__g->adj[i]);
    }
}

void freeNode(Node *__n) {
    if (__n) free(__n);
}

void freeList(Node *__n) {
    Node *it = __n;
    Node *prev = it;
    if(!__n) return;

    while(it) {
        prev = it;
        it = it->next;
        free(prev);
    }
}

enum VERTEX_STATUS { NEW_VERTEX, OLD_VERTEX };


int _add_vertex(Graph *__g, int __v1, int __v2) {

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

// Count how many vertices come after this one (and including it)
int countVertex(Vertex __v) {

    int count = 0;
    Vertex it = __v;
    while (it) {
        count ++;
        it = it->next;
    }

    return count;
}

// Count how many unvisited vertices are adjacent to this one, without modifying the
// visited array.
int countUnvisitedVertex(Vertex __v, bool *__visited) {
    int count = 0;
    Vertex it = __v;
    while(it) {
        if (!__visited[it->data - 1]) count++;
        it = it->next;
    }
    return count;
}

// Write a vertex to a file
void writeVertex(Vertex __v, FILE *__f, bool __di, bool *__visited) {

    Vertex it = __v;
    char *sep = " -- ";

    if (!it) {
        // fprintf(stderr, "Passed vertex is null!\n");
        fprintf(__f, "\n");
        return;
    }
    if (__di) sep = " -> ";


    // fprintf(stderr, "Going to count vertices...\n");
    int countUnvisited = countUnvisitedVertex(__v, __visited);
    fprintf(__f, "%s", sep);

    // printf("Counted vertices: %d\n", count);

    if (countUnvisited == 1) fprintf(__f, "%d", it->data); // if this is the only unvisited node in this adjacency list, print it
    if (countUnvisited > 1) {

        fprintf(__f, "{ ");

        while (it) {
            if (!__visited[it->data - 1]) {
                fprintf(__f, "%d ", it->data);
                // __visited[it->data - 1] = true; // This vertex will always get counted as visited.
            }
            it = it->next;
        }

        fprintf(__f, "}");
    }

    fprintf(__f, "\n");


}

// Scan from a stdinput
Graph *readGraph(const char *__filename, bool __digraph) {
// Graph *readGraph() {


    FILE *f = fopen(__filename, "r");
    int nvert = 0;
    int nedge = 0;
    int vert1 = 0; // Left hand vertex when reading the file
    int vert2 = 0; // Right hand vertex when reading in the graph


    char line[256];

    // get the number of vertices;
    if (fgets(line, sizeof(line), f)) sscanf(line, "%d", &nvert);
    if (fgets(line, sizeof(line), f)) sscanf(line, "%d", &nedge);

    // printf("Creating a new")
    Graph *g = newGraph(nvert, nedge, __digraph);

    while (fgets(line, sizeof(line), f)) {

        sscanf(line, "%d %d", &vert1, &vert2);
        // sscanf(line, "", &vert2);

        // printf("line: %s; v1: %d, v2: %d\n", line, vert1, vert2);

        addVertex(g, vert1, vert2);
        // addVertex(g, vert2, vert1);

        // printf("%s", line);

    }

    // sscanf("%d\n", &nvert);
    // scanf("%d\n", &nedge)
    fclose(f);
    // while()

    return g;
}
// Initialize an array of booleans to false to check if they've been visited or not
bool *visitedArray(Graph *__g) {

    bool *visited = (bool *) calloc(sizeof(bool), __g->nvert);
    return visited;

}

bool visited(Vertex __v, bool *__visited) {
    return __visited[__v->data - 1];
}

// Create a dot output of the loaded graph to be visualized with graphviz
int createDot(Graph *g, const char *__filename) {

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



// Depth-first search involves using an array to keep track of visited
// check if __v and __s are connected via Depth-First, a recursive routine
// that uses a matrix to store already visited or not
// Accessing with a[1] is the first element...
// bool dfs(Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __s, bool *__visited) {
//     // base case, we didnt find it
//     // if (__g->adj[__v - 1] == NULL) return false;
//     __visited[__v - 1] = true; // Set this node as visited

//     // We can use recursion to check the depth of a tree first
//     // if (__v == __s) {
//     //     __visited[__s - 1] = true;
//     //     return true;
//     // }
//     if (__v == __s) return true;

//     Vertex it = __g->adj[__v - 1];
//     while (it) {
//         // recursion for EVERY node connected to this vertex!
//         if (!visited(__v, __visited)) return dfs(__g, it->data, __s, __visited);
//         it = it -> next;
//     }

//     return false;

// }

// Wrapper routine to access the adjacency list of vertex v
Vertex graphAdj(Graph *__g, Vertex __v) {
    return __g->adj[__v->data - 1];
}

// This routine has a bool array that is passed down between successive calls.
void _dfsVisualize(Graph *__g, int __v, bool *__visited) {

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

// __v is the starting vertex
void dfsVisualize(Graph *__g, int __v) {
    bool *visited = visitedArray(__g);
    _dfsVisualize(__g, __v, visited);
    free(visited);
}

// Return true iff the two elements are connected
bool dfsConnected(Graph *__g, int __v, int __u) {




}

// bool areConnected(Graph *__g, GRAPH_TYPE __v, GRAPH_TYPE __s) {
//     return dfs(__g, __v, __s, visitedArray(__g));
// }



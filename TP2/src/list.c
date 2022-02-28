#include "list.h"

Node *newNode(GRAPH_TYPE val) {

    Node *n = (Node *) malloc(sizeof(Node));
    n->data = val;
    n->next = NULL;
    return n;
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

// Used to duplicate a list of vertices in a graph's adjacency list
Node *duplicateList(Node *__n) {

    if(!__n) return NULL;

    Vertex dupl = newNode(__n->data);

    Vertex it = __n->next;
    Vertex it_dup = dupl;
    while(it) {

        it_dup->next = newNode(it->data);

        it_dup = it_dup->next;
        it = it->next;
    }

    return dupl;
}

void printVertices(Vertex __vs) {

    Vertex it = __vs;
    while (it) {
        printf("%d ", it->data);
        it = it->next;
    }
    printf("\n");

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

// Wrapper routine to check whether or not a vertex has already been visited.
// It's really fing annoying that the vertices are read in starting at 1 and not 0.
// Maybe I'll change that later so that the vertices are internally stored starting
// at 0 and only when I print to the screen will I increment the value
bool visited(Vertex __v, bool *__visited) {
    return __visited[__v->data - 1];
}

// Write a vertex to a file
void writeVertexSubgraph(int __root, Vertex __v, FILE *__f, bool __di, bool *__visited) {

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
    fprintf(__f, "%d %s", __root, sep);

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
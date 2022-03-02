#include "graph.h"
#include <assert.h>


void t_stronglyConnected();

int main() {

    Graph *g = newGraph(10, 10, true);

    printGraph(g);

    addEdge(g, 4, 3);
    addEdge(g, 8, 1);

    printGraph(g);

    int v = getVertex(g);

    printf("V: %d\n", v);

    t_stronglyConnected();

    return 0;
}

void t_stronglyConnected() {

    printf("================= Strongly connected =================\n");

    Graph *g = newGraph(8, 0, true);

    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    addEdge(g, 3, 4);
    addEdge(g, 4, 1);

    addEdge(g, 4, 5);
    addEdge(g, 5, 6);
    addEdge(g, 6, 7);
    addEdge(g, 7, 5);

    addEdge(g, 7, 8);

    createDot(g, "t_stronglyConnected.dot");

    Graph *sg = stronglyConnected(g);

    printGraph(sg);

    printf("Started search at %d\n", getVertex(g));

    createDot(sg, "t_stronglyConnectedComponents.dot");

}
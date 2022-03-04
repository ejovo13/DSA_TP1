#include "graph.h"
#include <assert.h>


void t_stronglyConnected();
void t_parti3();
void t_hasBridge();
void t_removeMirror();

int main() {

    Graph *g = newGraph(10, 10, true);

    printGraph(g);

    addEdge(g, 4, 3);
    addEdge(g, 8, 1);

    printGraph(g);

    int v = getVertex(g);

    printf("V: %d\n", v);

    t_stronglyConnected(); // The strongly connected components function seems to be working!!
    t_parti3();
    t_hasBridge();
    t_removeMirror();

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

    Graph *rev = reverseGraph(g);

    createDot(rev, "reverse_graph.dot");

    Graph *sg = stronglyConnected(g);

    printGraph(sg);

    printf("Started search at %d\n", getVertex(g));

    createDot(sg, "t_stronglyConnectedComponents.dot");

    g = readGraph("digraph-1.txt", true);

    createDot(g, "t_digraph.dot");

    sg = stronglyConnected(g);

    createDot(sg, "t_digraph_sg.dot");


    printGraph(g);


}

void t_parti3() {

    printf("\n=============== Parti 3 ==============\n");

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

    printGraph(g);

    printf("Removing connection between 1 and 2\n");

    Graph *g2 = removeEdge(g, 1, 2);

    printf("Address of g2->adj[0]: %x\n", g2->adj[0]);

    printGraph(g2);

    g2 = removeEdge(g2, 4, 1);

    printGraph(g2);

}

void t_hasBridge() {

    Graph *g = readGraph("migraph-1.txt", false);

    printGraph(g);

    createDot(g, "my-graph.dot");

    printf("G has bridge: %d\n", hasBridge(g));

    addEdge(g, 1, 4);
    addEdge(g, 2, 1);

    printf("G has bridge: %d\n", hasBridge(g));



}

void t_removeMirror() {

    Graph *g = readGraph("migraph-1.txt", false);

    printGraph(g);

    printGraph(removeMirrorConnection(g));



}
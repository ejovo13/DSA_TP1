#include "graph.h"

int main() {

    // Graph *g = readGraph("./instances/digraph-1.txt", false);
    Graph *dg1 = readGraph("./instances/digraph-1.txt", true);
    Graph *g1 = readGraph("./instances/graph-1.txt", false);
    Graph *g2 = readGraph("./instances/graph-2.txt", false);
    Graph *g3 = readGraph("./instances/graph-3.txt", false);
    Graph *g4 = readGraph("./instances/graph-4.txt", false);
    Graph *g5 = readGraph("./instances/graph-5.txt", false);

    // Graph *e = readGraph("./instances/my_graph.txt", false);
    Graph *e = readGraph("./instances/my_graph.txt", false);

    // createDot(dg1, "dg1.dot");
    // createDot(g1, "g1.dot");
    // createDot(g2, "g2.dot");
    // createDot(g3, "g3.dot");
    // createDot(g4, "g4.dot");
    // createDot(g5, "g5.dot");
    createDot(e, "e.dot");

    int v = 1;
    int u = 3;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));

    v = 5;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    v = 4;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    v = 13;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    v = 12;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    u = 6;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    u = 9;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    v = 11;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    u = 14;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    v = 14;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    u = 16;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));

    v = 13; u = 3;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));
    u = 7;
    printf("Are %d and %d connected: %d\n", v, u, dfsConnected(e, v, u));

    printGraph(e);
    dfsVisualize(e, 13);

    printf("============= Finished DFS ==============\n");

    bfsVisualize(e, 1);
    printf("============= Finished BFS ==============\n");

    bfsVisualize(e, 2);
    printf("============= Finished BFS ==============\n");

    // Queue *q = newQueue();

    Graph *one = subgraph(e, 1);
    printGraph(one);

    createDotSubgraph(one, "e_1.dot");

    Graph *ten = subgraph(e, 10);
    createDotSubgraph(ten, "e10.dot");

    printf("Is e connected: %d\n", isConnected(e));
    printf("Is g1 connected: %d\n", isConnected(g1));
    printf("Is g2 connected: %d\n", isConnected(g2));
    printf("Is g3 connected: %d\n", isConnected(g3));
    printf("Is g4 connected: %d\n", isConnected(g4));
    printf("Is g5 connected: %d\n", isConnected(g5));


    printf("============================================\n");

    printGraph(dg1);

    Graph *rev = reverseGraph(dg1);

    printGraph(rev);

    createDot(rev, "rev.dot");


    return 0;
}
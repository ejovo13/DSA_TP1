#include "graphes.c"

int main() {

    // Graph *g = readGraph("./instances/digraph-1.txt", false);
    // Graph *dg1 = readGraph("./instances/digraph-1.txt", true);
    // Graph *g1 = readGraph("./instances/graph-1.txt", false);
    // Graph *g2 = readGraph("./instances/graph-2.txt", false);
    // Graph *g3 = readGraph("./instances/graph-3.txt", false);
    // Graph *g4 = readGraph("./instances/graph-4.txt", false);
    // Graph *g5 = readGraph("./instances/graph-5.txt", false);

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
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));

    v = 5;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    v = 4;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    v = 13;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    v = 12;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    u = 6;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    u = 9;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    v = 11;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    u = 14;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    v = 14;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));
    u = 16;
    printf("Are %d and %d connected: %d\n", v, u, areConnected(e, v, u));



    printGraph(e);



    return 0;
}
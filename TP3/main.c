// #include "src/tree.c"
// #include "include/ejovo_rand.h"
// #include "ejovo/ejovo_rand.c"
#include "tree.h"
#include "ejovo_matrix.h"

void t_linspace();
void t_range();
void partie_b();
double raisedBy10(double);
Vector *logspace(double, double, int);


int main() {

    ejovo_seed();

    BinTree *root = newBinTree(10);
    // addKeyBST(bst, 3);
    // addKeyBST(bst, 2);
    // addKeyBST(bst, 3);
    // addKeyBST(bst, 5);
    // addKeyBST(bst, 13);

    addKeyBST(root, 1); // Adds 1 to the left of 10
    addKeyBST(root, 0); // Adds 0 to the left of 10
    addKeyBST(root, 13); // Adds 13 to the left of 10
    addKeyBST(root, 5); // Adds 5 to the right of 1
    addKeyBST(root, 8);
    addKeyBST(root, 25);
    addKeyBST(root, 3);

    // createDotBST(root, "test.dot");

    printf("After 10: %d\n", getNextKey(root)->key);
    printf("After 13: %d\n", getNextKey(getNextKey(root))->key);

    BinTree *one = getKey(root, 1);

    printf("one: %x\n", one);
    printf("one->key: %d\n", one->key);

    // BinTree *two = getKey(root, 2);

    // BinTree *three = getKey(root, 3);

    // removeKey(root, 3);
    // createDotBST(root, "test_rem3.dot");

    // deleteKey(root, 13);
    // createDotBST(root, "test_rem13.dot");

    // removeKey(root, 1);
    // createDotBST(root, "test_rem1.dot");



    // removeKey(root, 10);
    // // removeHead(&root);
    // createDotBST(root, "test_rem10.dot");

    // printf("three: %x\n", three); // it appears that searching for a node that doesnt exist returns null.

    int *fischer = fischer_yates(100);

    printf("{");
    for (int i = 0; i < 99; i++) {
        printf("%d, ", fischer[i]);
    }
    printf("%d}", fischer[99]);

    root = createRandomTree(140);

    // createDotBST(root, "random.dot");

    BinTree *rando = createRandomTree(10);
    // createDotBST(rando, "rando.dot");

    printf("Height of rando: %d\n", heightBST(rando));
    printf("Deq factor of rando: %d\n", deqFactor(rando));

    printf("Depth(rando, 10): %d\n", depthKey(rando, 10));

    t_linspace();
    t_range();
    partie_b();


    return 0;
}


// Let's start gathering some data for different random variables
void t_linspace() {

    printf("Starting linspace!\n");

    Matrix *v = linspace(0, 100, 10);
    Matrix_print(v);

    Matrix_free(v);

}

void t_range() {


    Vector *v = range(1, 100, 3);

    Matrix_print(v);

    Matrix_free(v);


}

void partie_b() {

    #define MAX_POWER 4 // 10^6 is already really SLOW!!!
    #define N_GRAPHS 50

    int length = MAX_POWER * 30;

    Vector *N = logspace(1, MAX_POWER, length);

    Matrix_print(N);

    Vector *mean_height = Matrix_new(1, length);
    Vector *mean_deq = Matrix_new(1, length);
    Vector *log2_n = Matrix_new(1, length);

    int n = 0;
    BinTree *root = NULL;
    int cum_height = 0;
    int cum_deq = 0;

    // Now iterate through the different N values
    for (int i = 0; i < length; i++) {


        n = Vector_at(N, i);
        printf("Testing n = %d\n", n);
        cum_height = 0;
        cum_deq = 0;

        // Now generate 50 graphs
        for (int j = 0; j < N_GRAPHS; j++) {

            root = createRandomTree(n);

            cum_height += heightBST(root);
            cum_deq += deqFactor(root);

        }

        Vector_set(mean_height, i, (double) cum_height / N_GRAPHS);
        Vector_set(mean_deq, i, (double) cum_deq / N_GRAPHS);
        Vector_set(log2_n, i, log2(n));

    }

    // Now I've got the average Height and
    // The average disequilibrium factor

    printf("Average height:\n");
    Matrix_print(mean_height);
    printf("Average deq:\n");
    Matrix_print(mean_deq);
    printf("log2(n)");
    Matrix_print(log2_n);


}

double raisedBy10(double __input) {
    return pow(10, __input);
}

Vector *logspace(double __start, double __end, int __n) {

    Vector *exp = linspace(__start, __end, __n);

    Vector *out = Vector_map(exp, raisedBy10);

    Matrix_free(exp);

    return out;
}
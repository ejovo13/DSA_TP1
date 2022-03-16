// #include "src/tree.c"
// #include "include/ejovo_rand.h"
// #include "ejovo/ejovo_rand.c"
#include "tree.h"
#include "ejovo_matrix.h"
#include "ejovo_dataframe.h" // used to store values and make writing to csv super easy.

void partie_a();
void partie_b();


int main() {

    ejovo_seed(); // Seed the random number generator


    return 0;
}

void partie_a() {

    BinTree *root = newBinTree(10);

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

}



void partie_b() {

    #define MAX_POWER 2 // 10^6 is already really SLOW!!!
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
    printf("log2(n)\n");
    Matrix_print(log2_n);

}
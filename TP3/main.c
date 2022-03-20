// #include "src/tree.c"
// #include "include/ejovo_rand.h"
// #include "ejovo/ejovo_rand.c"
#include "tree.h"
#include "ejovo_matrix.h"
#include "ejovo_rand.h"
#include "ejovo_dataframe.h" // used to store values and make writing to csv super easy.
#include "list.h"

void partie_a();
void partie_b();
void partie_c(const int n, const int valMin, const int valMax);

// #define N 10
// #define valMin 0
// #define v

int main() {

    ejovo_seed(); // Seed the random number generator

    // partie_a();
    // partie_b();
    partie_c(10, 0, 50);

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





void partie_c(const int n, const int valMin, const int valMax) {

    // Instaniate the machinery I need to tackle this problem

    // generer uniformement SIZE_BST cles, toutes entre MIN_VALUE et MAX_VALUE
    Vector *rando = Vector_random(n, valMin, valMax);
    List *list = newList();
    BinTree *tree = newBinTree(Vector_first(rando)); // instantiate the tree with the first element
    appendList(list, tree->key);

    int size_tree = 1;

    int to_remove = -1;
    int to_add    = -1;
    Node *rem     = NULL;
    double p      = -1;

    for (int i = 1; i < n; i++) {

        printList(list);
        char buff[20] = {0};

        sprintf(buff, "tree%d.dot", i);

        printf("writing to %s\n", buff);
        createDotBST(tree, buff);
        printf("Starting iteration with i: %d, size_tree: %d, lenList: %d\n", i, size_tree, lenList(list));
        printf("real size: %d\n", countNodes(tree));

        // if size is 0
        if (size_tree == 0) {

            // then add to the tree.
            to_add = Vector_at(rando, i);
            printf("Tree is empty, adding %d\n", to_add);

            appendList(list, to_add);

            if (tree = NULL) {
                tree = newBinTree(to_add);
            } else {
                addKeyBST(tree, to_add);
            }

            size_tree ++;
            continue;
        }

        // generer a number between 0 and 1
        p = unifd(0, 1);
        // printf("Generating a random number p: %lf\n", p);

        if (p < 0.5) {
            // proceed to insert
            add_element:
            to_add = Vector_at(rando, i);

            if (isElementList(list, to_add)) {
                // then don't add this number right?
                i++;
                printf("Element already exists in the tree\n");
                goto add_element;
            } else {
                // add the element
                appendList(list, to_add);
                addKeyBST(tree, to_add);
                size_tree ++;

                continue;
            }

        } else {
            // Deal with single element
            if (size_tree == 1) {
                printf("Removing singular element\n");
                free(tree);
                tree = NULL;
                size_tree = 0;
                free(extractNode(list, 0));
                continue;
            }

            // otherwise remove a random key
            // generate the index from 0 to lenList() - 1 for a key to remove
            int index = unif(0, size_tree - 1);

            // printList(list);
            rem = extractNode(list, index);
            to_remove = rem->val;
            // free(extractNode(list, index)); //remove node from list, free it at the same time

            printf("Removing %d -> ", to_remove);
            printList(list);

            // printf("Is %d even in my graph?: %d\n", to_remove, isElement(tree, to_remove));
            size_tree --;
            printf("removed key: %d\n", removeKey(tree, to_remove)->key);

            continue;

        }




    }





}

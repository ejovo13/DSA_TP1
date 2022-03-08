// #include "src/tree.c"
// #include "include/ejovo_rand.h"
// #include "ejovo/ejovo_rand.c"
#include "tree.h"
#include "ejovo_matrix.h"
#include "ejovo_dataframe.h"

void t_linspace();
void t_range();
void partie_b();
void t_filter();

bool lessThan10(double x);

void t_string() {


    String *s = newStr("This is my new str");

    println(s);

    printf("String has length: %d\n", s->len);

    Chain *c = newChainVar(3, "Hi", "Hello", "Sup");

    printChain(c);

}

// A "Space" is simply a list of Vectors
// whereas a chain, in general, should take care of memory, a Space
// DOES NOT TOUCH the memory of the underlying matrices.


void t_space() {


    ejovo_seed();

    Vector *v1 = Matrix_rand(10, 1);
    Vector *v2 = Matrix_rand(10, 1);
    Vector *v3 = Matrix_rand(10, 1);

    Space *s = newSpaceVar(3, v1, v2, v3);
    // newSpaceVar(v1);
    printSpace(s);

    printf("Is the space uniform? %d\n", uniformSpace(s));

}

void printMax(const Vector *__v) {
    // let's simply print out to the console the max of each row
    printf("max of vec: %7.3lf\n", max(__v));
}

void printMin(const Vector *__v) {
    printf("min of vec: %7.3lf\n", min(__v));
}

void t_dataframe() {

    int N = 10000;

    Vector *v1 = Matrix_rand(N, 1);
    Vector *v2 = Matrix_rand(N, 1);
    Vector *v3 = Matrix_rand(N, 1);
    Vector *v4 = Matrix_rand(N, 1);
    Vector *v5 = Matrix_rand(N, 1);
    Vector *v6 = Vector_runif(N, 0, 10);

    Space *s = newSpaceVar(6, v1, v2, v3, v4, v5, v6);
    // Space *s = newSpaceVar(5, v1, v2, v3, v4, v5);
    Chain *c = newChainVar(6, "v1", "v2", "v3", "v4", "v5", "v6");

    DataFrame *df = newDataFrame(c, s);
    printDataFrame(df);

    // foreachString(df, println);

    // foreachVector(df, printMax);
    // foreachVector(df, printMin);

    writeCSV(df, "test.csv");

    Vector *test1 = getColDF(df, 5);
    Vector *test2 = getColDF(df, 5);

    // Matrix_print(test1);
    Vector_print_head(test1, 5);
    Vector_print_head(test2, 5);

    printf("correlation coefficient between v6 and itself: %lf\n", cor(test1, test2));

    printf("var of test1: %lf\n", var(test1));
    printf("mean of test1: %lf\n", mean(test1));
    printf("mean_sqaured: %lf\n", mean_squared(test1));

    printf("first raw moment of test1: %lf\n", rmoment(test1, 1));
    printf("second raw moment %lf\n", rmoment(test1, 2));



    printf("zeroth central moment of test1: %lf\n", cmoment(test1, 0));
    printf("first central moment of test1: %lf\n", cmoment(test1, 1));
    printf("second central moment of test1: %lf\n", cmoment(test1, 2));
    printf("third central moment of test1: %lf\n", cmoment(test1, 3));



    Vector *t = Matrix_from((double []) {1, 2, 3}, 3, 1);

    Matrix_print(t);

    mathadexp(t, 2);
    Matrix_print(t);

    // Print correlation between row 6 and itself.
    // Need routines to access a pointer to the nth column

}

void t_demo() {

    Matrix *m = Matrix_value(10, 10, 5);

    Matrix_print(m);

    Matrix *A = Matrix_rand(4, 3);
    Matrix *B = Matrix_random(8, 13, 0, 200);

    Matrix_print_fixed(A);
    Matrix_print_fixed(B);

    Matrix_reset(&A);
    Matrix_reset(&B);

    Vector *v = Vector_runif(100, -3, 8);
    // Vector_print_as_row(
    // )

    Vector_print_head(v, 15);

    v = Vector_linspace(0, 25, 9);

    Matrix_print_fixed(v);

    int n = 100;

    Chain *c = newChainVar(3, "Discrete uniform    ", "Continuous Uniform  ", "Standord Normal     ");
    Space *s = newSpaceVar(3, Vector_rand(n), Vector_runif(n, -5, 5), Vector_rnorm(n, 0, 1));

    DataFrame *df = newDataFrame(c, s);

    printDataFrame(df);

    printf("Mean: %lf\n", mean(getColDF(df, 2)));
    printf("std:  %lf\n",  std(getColDF(df, 2)));

    Vector *rnorm = Vector_rnorm(1000, 0, 1);    // 1000 samples of X ~ N(0, 1)
    Vector *runif = Vector_runif(1000, -10, 10); // 1000 samples of X ~ U(-10, 10)
    Vector *rexp  = Vector_rexp (1000, 3);     // 1000 samples of an exponential distribution with rate parameter `1/3`

    Space *root_alt = newSpaceVar(3, rnorm, runif, rexp);
    Chain *data_desc = newChainVar(3, "X ~ N(0 1)", "Y ~ U(-10 10)", "Z ~ exp(rate = 3)");

    // println(data_desc->data);

    DataFrame *d = newDataFrame(data_desc, root_alt);
    printDataFrame(d);

    writeCSV(d, "df.csv");
    // Vector_
}



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

    t_filter();
    t_string();
    t_space();
    t_dataframe();
    t_demo();

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

bool gt50_fn(double x) {
    return x > 50;
}

void t_filter() {


    Vector *v = range(1, 100, 3);

    Vector *gt50 = filter(v, gt50_fn);

    printf("Original list: \n");
    Matrix_print(v);
    printf("Greater than 50:\n");
    Matrix_print(gt50);

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
    printf("log2(n)");
    Matrix_print(log2_n);


}

bool lessThan10(double x) {
    return x < 10;
}


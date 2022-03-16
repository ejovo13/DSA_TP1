// This file contains some random functions that I wrote while developping the Dataframe
// and other structures for the ejovo library.

#include "ejovo_matrix.h"
#include "ejovo_dataframe.h"


void t_filter();
void t_linspace();
void t_range();


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

// Let's start gathering some data for different random variables
void t_linspace() {

    printf("Starting linspace!\n");

    Matrix *v = linspace(0, 100, 10);
    Matrix_print(v);

    Matrix_free(v);

}

bool lessThan10(double x) {
    return x < 10;
}

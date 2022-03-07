// #include "src/tree.c"
// #include "include/ejovo_rand.h"
// #include "ejovo/ejovo_rand.c"
#include "tree.h"
#include "ejovo_matrix.h"
#include <stdarg.h>

void t_linspace();
void t_range();
void partie_b();
double raisedBy10(double);
Vector *logspace(double, double, int);
typedef bool (* predicate_fn) (double);
void t_filter();

bool lessThan10(double x);

Vector *filter(const Vector *__v, predicate_fn __pred);

/* I'm gonna get a little out of hand here and implement some additional data types that I will eventually be able to
use as a data.frame like object */

typedef struct str_t {

    char *str;
    int len;

} String;

String *newStr(const char *__str) {

    String *str = (String *) malloc(sizeof(String));

    if (!str) return NULL;

    // First thing to do is count the number of characters in the string.
    int len = 0;

    while (__str[len] != '\0') {
        len++;
    }

    // Now allocate my OWN memory of the string
    str->str = (char *) malloc(sizeof(char) * len + 1);

    // it's way faster to just copy over the bytes.

    memcpy(str->str, __str, len);
    // for (int i = 0; i < len; i++) {
    //     str->str[i] = __str[i];
    // }

    // str->str[len] = '\0';
    str->len = len;

    return str;

}

void print(const String *__str) {
    printf("%s", __str->str);
}

void println(const String *__str) {
    printf("%s\n", __str->str);
}

String *copyStr(const String *__str) {

    String *str = (String *) malloc(sizeof(String));

    str->len = __str->len;
    memcpy(str->str, __str->str, __str->len);
    return str;

}

void freeStr(String **__str) {

    if (!*__str) return;

    if ((*__str)->str) free((*__str)->str);

    *__str = NULL;
}

typedef struct chain_t {

    String *data;
    struct chain_t *next;

} Chain;

Chain *newChain(const char *__str) {

    Chain *chain = (Chain *) malloc(sizeof(Chain));

    chain->next = NULL;
    chain->data = newStr(__str);

    // Duplicate the string so that chain is in charge of the strings that it carries
    return chain;
}

// Return the root node of the chain just in case the chain is initially empty
Chain *appendChain(Chain *__chain, const char *__str) {

    if (!__chain) return newChain(__str);

    Chain *it = __chain;

    while (it->next) {

        it = it->next;

    }

    it->next = newChain(__str);
    return __chain;
}

int sizeChain(const Chain *__chain) {

    int count = 0;
    const Chain *it = __chain;

    while (it) {
        count ++;
        it = it->next;
    }

    return count;
}

void printChain(const Chain *__chain) {

    if (!__chain) {
        printf("Chain does not exist!\n");
        return;
    };

    printf("Chain with size %d:\n", sizeChain(__chain));

    const Chain *it = __chain;
    while (it) {

        printf("%s -> ", it->data->str);
        it = it->next;
    }
    printf("\n");
}

Chain *newChainVar(int __count, ...) {

    va_list ptr;

    va_start(ptr, __count);
    char *s = va_arg(ptr, char *);

    if (__count == 0) return NULL;

    Chain *chain = newChain(s);

    for (int i = 1; i < __count; i++) {
        appendChain(chain, va_arg(ptr, char *));
    }

    return chain;
}

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
typedef struct space_t {

    Vector *data;
    struct space_t *next;

} Space;

Space *newSpace(Vector *__v) {

    Space *space = (Space *) malloc(sizeof(Space));

    space->data = __v;
    space->next = NULL;

    return space;
}

Space *appendSpace(Space *__space, Vector *__v) {

    if (!__space) return newSpace(__v);

    Space *it = __space;

    while (it->next) {

        it = it->next;

    }

    it->next = newSpace(__v);
    return __space;
}

Space *newSpaceVar(int __count, ...) {

    va_list ptr;

    va_start(ptr, __count);
    Vector *v = va_arg(ptr, Vector *);

    if (__count == 0) return NULL;

    Space *space = newSpace(v);

    for (int i = 1; i < __count; i++) {
        appendSpace(space, va_arg(ptr, Vector *));
    }

    return space;
}


// Count the number of vectors in the space
int dimSpace(const Space *__space) {

    const Space *it = __space;
    int count = 0;

    while (it) {
        count ++;
        it = it->next;
    }

    return count;
}

// Print the number and addresses of the vectors in this space
void printSpace(const Space *__space) {

    if (!__space) {
        printf("Space does not exist\n");
        return;
    }

    printf("Space of dim %d\n", dimSpace(__space));

    const Space *it = __space;

    while (it) {
        printf("%x -> ", it->data);
        it = it->next;
    }
    printf("\n");
}

// verify that the dimensions of the matrices in __space are all the same
bool uniformSpace(const Space *__space) {

    if (!__space) return false;

    if (!__space->next) return true; // There is only one matrix

    const Space *it = __space;

    while (it->next) {

        if (!Matrix_comp_add(it->data, it->next->data)) return false;
        it = it->next;
    }

    return true;
}

// Now I want a structure that is a pointer to a chain and a pointer to a space.
// This object will be used for data exportation

// I should also be able to add a new column to a data frame
// when given a string and a conforming vector
typedef struct data_frame_t {

    Chain *chain;
    Space *space;

} DataFrame;

DataFrame *newDataFrame(Chain *__chain, Space *__space) {

    DataFrame *df = (DataFrame *) malloc(sizeof(DataFrame));

    if (!df) return NULL;

    // Verify that the chain and the space have the same size
    if (sizeChain(__chain) != dimSpace(__space)) {
        printf("Error: chain and space are different sizes!!\n");
        return NULL;
    }

    if (!uniformSpace(__space)) {
        printf("Error: matrices in __space are not the same size!!\n");
        return NULL;
    }

    df->chain = __chain;
    df->space = __space;

    return df;
}

int dimDataFrame(const DataFrame *__df) {
    return dimSpace(__df->space);
}

typedef void (* string_vector_fn) (String *, Vector *);
typedef void (* string_fn) (String *);
typedef void (* vector_fn) (Vector *);

void foreachSV(const DataFrame *__df, string_vector_fn fn) {

    if (!__df) return;

    const Chain *cit = __df->chain;
    const Space *sit = __df->space;

    while (cit && sit) {

        fn(cit->data, sit->data); // Call the function for some combination of chain and space

        cit = cit->next;
        sit = sit->next;

    }
}

void foreachString(const DataFrame *__df, string_fn fn) {

    if (!__df) return;

    const Chain *cit = __df->chain;

    while (cit) {

        fn(cit->data);

        cit = cit->next;

    }
}

void foreachVector(const DataFrame *__df, vector_fn fn) {

    if (!__df) return;

    const Space *sit = __df->space;

    while (sit) {

        fn(sit->data);
        sit = sit->next;
    }
}

// print the name of the col and then the first __nel elements
void printDataFrameCol(const String *__str, const Vector *__v) {

    print(__str);
    printf(": ");

    int nel = 10;
    // Print the first __nel elements
    int n = nel < Matrix_size(__v) ? nel : Matrix_size(__v); // get the smaller element

    printf("{");

    for (int i = 0; i < n - 1; i++) {
        printf("%7.3lf, ", Vector_at(__v, i));
    }

    printf("%7.3lf}\n", Vector_at(__v, n - 1));

}

void printDataFrame(const DataFrame *__df) {

    // first print the dimension and number of cols

    int nobs = Matrix_size(__df->space->data);

    printf("Df with %d observations and %d colums\n", nobs, dimDataFrame(__df));
    foreachSV(__df, printDataFrameCol);

}

void writeCSV(const DataFrame *__df, const char *__filename) {

    if (!__df) return;

    FILE *f = fopen(__filename, "w");

    // the first line is all of the strings, separated with a column
    Chain *it = __df->chain;

    while (it->next) {

        fprintf(f, "%s,", it->data->str);
        it = it->next;
    }
    fprintf(f, "%s\n", it->data->str);

    // now iterate through all of vectors, accessing the ith print at each point.

    int size = Vector_size(__df->space->data);

    for (int i = 0; i < size; i++) {

        Space *it = __df->space;

        while (it->next) {

            fprintf(f, "%lf,", Vector_at(it->data, i));
            it = it->next;
        }
        fprintf(f, "%lf\n", Vector_at(it->data, i));
    }

    fclose(f);

}

// Return a pointer to the column in question
// Columns shall be ZERO indexed
Vector *getColDF(const DataFrame *__df, int __j) {

    if (!__df) return NULL;

    const Space *it = __df->space;

    while (it && __j > 0) {
        it = it->next;
        __j --;
    }

    if (it) return it->data;
    else return NULL;
}

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

double raisedBy10(double __input) {
    return pow(10, __input);
}

Vector *logspace(double __start, double __end, int __n) {

    Vector *exp = linspace(__start, __end, __n);

    Vector *out = Vector_map(exp, raisedBy10);

    Matrix_free(exp);

    return out;
}


typedef bool (* predicate_fn) (double);

bool lessThan10(double x) {
    return x < 10;
}

Vector *filter(const Vector *__v, predicate_fn __pred) {

    // First I need to pass through the vector to see how many times the predicate is true...
    int count = 0;

    for (int i = 0; i < Vector_size(__v); i++) {

        if (__pred(Vector_at(__v, i))) count++;

    }

    Vector *out = Matrix_new(1, count);

    int j = 0; // index of out
    for (int i = 0; i < Vector_size(__v); i++) {
        if (__pred(Vector_at(__v, i))) {
            Vector_set(out, j, Vector_at(__v, i));
            j ++;
        }
    }

    return out;

}
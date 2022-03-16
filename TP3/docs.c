// This file contains the routines that I used to create visualizations for the
// documentation for this project.

#include "tree.h"

int main() {



    int *fischer = fischer_yates(100);

    printf("{");
    for (int i = 0; i < 99; i++) {
        printf("%d, ", fischer[i]);
    }
    printf("%d}", fischer[99]);

    BinTree *root = createRandomTree(140);


    return 0;
}
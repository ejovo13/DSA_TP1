#include "qsort.h"

#define N 100

int main() {
    ejovo_seed();

    int *list = unsorted_list(N);
    print_list(list, N);



    quicksort(list, 0, N - 1);

    // exchange(list, 0, N - 1);

    print_list(list, N);

    free(list);

    return 0;
}
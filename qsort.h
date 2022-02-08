#include "ejovo/ejovo_rand.h"

#include <stdlib.h>
#include <stdbool.h>

// Need two simple functions, one that partitions and another that actually performs the quick sort

// Assume that we are sorting integer arrays


// We are going to qsort in-place

void exchange(int *__arr, int __i, int __j) {
    int temp = __arr[__i];
    __arr[__i] = __arr[__j];
    __arr[__j] = temp;
}

// Lomuto partition scheme
int partition(int *__arr, int __lo, int __hi) {

    int pivot = __arr[__hi]; // start at the high end
    int i = (__lo - 1); // temp pivot index

    for (int j = __lo; j < __hi; j++) {
        if (__arr[j] <= pivot) {
            i ++;
            exchange(__arr, i, j);
        }
        // exchange(__arr, i, j);
    }

    i = i + 1; // Move the pivot element to the correct pivot position
    exchange(__arr, i, __hi); // Make the final proper swap
    return i; // The index of the pivot
}

void swapi(int *__a, int *__b) {
    // printf("Swapping %d, %d\n", *__a, *__b);
    int t = *__a;
    *__a = *__b;
    *__b = t;
}

// Hoarde partition scheme
// still not working for some reason...
int hpartition(int *__arr, int __lo, int __hi) {

    printf("entering partition\n");
    print_list(__arr + __lo, __hi - __lo + 1);

    int pivot = __arr[(int) floor((__hi + __lo) / 2)];
    printf("pivot: %d\n", pivot);

    // left index
    int i = __lo - 1;
    int j = __hi + 1;

    bool loop = true;

    while(loop) {
        do {
            i++;
        }
        while (__arr[i] < pivot);

        do {
            j--;
        }
        while (__arr[j] > pivot);

        if (i >= j) return j;

        // printf("i: %d, j: %d\t", i, j);
        swapi(__arr + i, __arr + j);

        // break;
    }
}


void quicksort(int *__arr, int __lo, int __hi) {

    if (__lo >= __hi || __lo < 0) return;

    int pi = partition(__arr, __lo, __hi);
    quicksort(__arr, __lo, pi - 1); // lower portion
    quicksort(__arr, pi + 1, __hi);
}

// Returns an unsorted list that from 0 .. __n - 1 shuffled via fisher-yates
int *unsorted_list(size_t __n) {

    int *list = (int*) malloc(sizeof(int) * __n);

    // initialise to [0, ... , __n - 1]
    for (int i = 0; i < __n; i++) {
        list[i] = i;
    }

    int rj; // random index between [i and _n - 1]

    // now shuffle it
    for (int i = 0; i < __n - 1; i++) {
        rj = unif(i, __n - 1);
        swapi(list + i, list + rj);
    }

    return list;

}

void print_list(const int *__list, size_t __n) {
    printf("{");
    for (size_t i = 0; i < __n - 1; i++) {
        printf("%d, ", __list[i]);
    }
    printf("%d}\n", __list[__n - 1]);
}
#include "ejovo/ejovo_rand.h"

#include <stdlib.h>
#include <stdbool.h>

// Need two simple functions, one that partitions and another that actually performs the quick sort

// Assume that we are sorting integer arrays


// We are going to qsort in-place
void print_list(const int *__list, size_t __n) {
    printf("{");
    for (size_t i = 0; i < __n - 1; i++) {
        printf("%d, ", __list[i]);
    }
    printf("%d}\n", __list[__n - 1]);
}

void inc(int *__a, int __val) {
    *__a += __val;
}

void exchange(int *__arr, int __i, int __j) {
    int temp = __arr[__i];
    __arr[__i] = __arr[__j];
    __arr[__j] = temp;
}

void exchange_count(int *__arr, int __i, int __j, int *__nech) {
    int temp = __arr[__i];
    __arr[__i] = __arr[__j];
    __arr[__j] = temp;
    inc(__nech, 1); // Once again, we only care about array assignments.
    // increment the number of exchanged by 1 per call
}

// Lomuto partition scheme
// test different partition schemes
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

int partition_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech) {

    int pivot = __arr[__hi]; // start at the high end
    int i = (__lo - 1); // temp pivot index

    for (int j = __lo; j < __hi; j++) {
        if (__arr[j] <= pivot) {
            i ++;
            exchange_count(__arr, i, j, __nech);
        }
        // every time we go through the for loop we are checking if __arr[j] <= pivot
        inc(__ncmp, 1);
    }

    i = i + 1; // Move the pivot element to the correct pivot position
    exchange_count(__arr, i, __hi, __nech); // Make the final proper swap
    return i; // The index of the pivot
}

// implemented in Algorithms textbook
int partition_a(int *__arr, int __lo, int __hi) {
// the __lo is assumed to be lower than __hi, I guess.
    int i = __lo;
    int j = __hi + 1;

    int v = __arr[__lo];

    while (true) {
        while (__arr[++i] < v) if (i == __hi) break; // these breaks are edge guards
        while (v < __arr[--j]) if (j == __lo) break;
        if (i >= j) break;
        exchange(__arr, i, j);
    }
    exchange(__arr, __lo, j);
    return j;
}

// implemented in Algorithms textbook
int partition_a_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech) {

    int i = __lo;
    int j = __hi + 1;

    int v = __arr[__lo];

    while (true) {
        while (__arr[++i] < v) {
            inc(__ncmp, 1);
            if (i == __hi) break;
        }
        while (v < __arr[--j]) {
            inc(__ncmp, 1);
            if (j == __lo) break;
        }
        if (i >= j) break;
        exchange_count(__arr, i, j, __nech);
    }
    exchange_count(__arr, __lo, j, __nech);
    return j;
}


void swapi(int *__a, int *__b) {
    // printf("Swapping %d, %d\n", *__a, *__b);
    int t = *__a;
    *__a = *__b;
    *__b = t;
}

// Hoarde partition scheme
// still not working for some reason...
// int hpartition(int *__arr, int __lo, int __hi) {

//     printf("entering partition\n");
//     print_list(__arr + __lo, __hi - __lo + 1);

//     int pivot = __arr[(int) floor((__hi + __lo) / 2)];
//     printf("pivot: %d\n", pivot);

//     // left index
//     int i = __lo - 1;
//     int j = __hi + 1;

//     bool loop = true;

//     while(loop) {
//         do {
//             i++;
//         }
//         while (__arr[i] < pivot);

//         do {
//             j--;
//         }
//         while (__arr[j] > pivot);

//         if (i >= j) return j;

//         // printf("i: %d, j: %d\t", i, j);
//         swapi(__arr + i, __arr + j);

//         // break;
//     }
// }

int partition_bis(int *__arr, int __lo, int __hi) {

    int i = __lo;
    int ipivot = __lo;
    int pivot = __arr[__lo];

    for (int j = __lo + 1; j <= __hi; j++) {
        if (__arr[j] <= pivot) {
            ipivot ++;
            if (j > ipivot) exchange(__arr, ipivot, j);
        }
    }

    if (i < ipivot) exchange(__arr, i, ipivot);
    return ipivot;
}

int partition_bis_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech) {

    int i = __lo;
    int ipivot = __lo;
    int pivot = __arr[__lo];

    for (int j = __lo + 1; j <= __hi; j++) {
        if (__arr[j] <= pivot) {
            ipivot ++;
            if (j > ipivot) exchange_count(__arr, ipivot, j, __nech);
        }
        inc(__ncmp, 1); // increment once per check
    }

    if (i < ipivot) exchange_count(__arr, i, ipivot, __nech);
    return ipivot;

}

int partition_tp(int *T, int deb, int fin) {

    int pivot = T[deb];
    int lambda = deb + 1;
    int rho = fin;

    bool comparer = true;

    while (comparer) {

        while(T[lambda] < pivot) {
            lambda ++;
            if (lambda >= fin) break; // If the left pointer is out of bounds of the rightside
        }

        while (T[rho] > pivot) {
            rho --;
            if (rho <= deb) break;
        }

        if (lambda < rho) exchange(T, lambda, rho);
        else break;
    }

    // exchange(T, deb, rho); // ORIGINAL HAD AN `IF (DEB < RHO)` here!!
    if (deb < rho) exchange(T, deb, rho); // ORIGINAL HAD AN `IF (DEB < RHO)` here!!
    return rho;
}

int partition_tp_count(int *T, int deb, int fin, int *ncmp, int *nech) {

    int pivot = T[deb];
    int lambda = deb + 1;
    int rho = fin;

    bool comparer = true;

    while (comparer) {

        while(T[lambda] < pivot) { // 1 comparison between arr elements
            lambda ++;
            inc(ncmp, 1);
            if (lambda >= fin) break; // If the left pointer is out of bounds of the rightside
        }

        while (T[rho] > pivot) { // a second comparison
            rho --;
            inc(ncmp, 1);
            if (rho <= deb) break;
        }

        if (lambda < rho) exchange_count(T, lambda, rho, nech);
        else break;
    }

    // exchange(T, deb, rho); // ORIGINAL HAD AN `IF (DEB < RHO)` here!!
    if (deb < rho) exchange_count(T, deb, rho, nech); // ORIGINAL HAD AN `IF (DEB < RHO)` here!!
    return rho;

}



void quicksort(int *__arr, int __lo, int __hi) {

    if (__lo >= __hi || __lo < 0) return;

    int pi = partition(__arr, __lo, __hi);
    quicksort(__arr, __lo, pi - 1); // lower portion
    quicksort(__arr, pi + 1, __hi);
}

// void quicksort_bis(int *__arr, int __lo, int __hi) {





// }

void quicksort_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech) {
    if (__lo >= __hi || __lo < 0) return;

    int pi = partition_count(__arr, __lo, __hi, __ncmp, __nech);
    quicksort_count(__arr, __lo, pi - 1, __ncmp, __nech);
    quicksort_count(__arr, pi + 1, __hi, __ncmp, __nech);
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



// return true if the integer array is in ascending order
bool verifier_tri(const int *__list, size_t __n) {
    for (size_t i = 1; i < __n; i++) {
        if (__list[i - 1] > __list[i]) return false;
    }
    return true;
}


// on some bossmode shit!!!!

typedef int (* partition_scheme) (int *, int, int);

void qs(int *__arr, partition_scheme __ps, int __lo, int __hi) {

    if (__lo >= __hi || __lo < 0) return;

    int pi = __ps(__arr, __lo, __hi);
    qs(__arr, __ps, __lo, pi - 1); // lower portion
    qs(__arr, __ps, pi + 1, __hi);

}

typedef int (* partition_scheme_count) (int *, int, int, int *, int *);

void qs_count(int *__arr, partition_scheme_count __ps, int __lo, int __hi, int *__ncmp, int *__nech) {

    if (__lo >= __hi || __lo < 0) return;

    int pi = __ps(__arr, __lo, __hi, __ncmp, __nech);
    qs_count(__arr, __ps, __lo, pi - 1, __ncmp, __nech);
    qs_count(__arr, __ps, pi + 1, __hi, __ncmp, __nech);
}



int *range(int __min, int __max, int __delta) {

    // first compute the number of elements
    int gap = __max - __min;
    int n = gap / __delta;

    int *r = (int *) malloc(sizeof(int) * n);
    r[0] = __min;

    for (int i = 1; i <= n; i++) {
        r[i] = __min + i * __delta;
    }

    return r;
}
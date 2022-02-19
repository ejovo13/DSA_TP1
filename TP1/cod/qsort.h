#pragma once

#include "ejovo/ejovo_rand.h"
#include <stdbool.h>
#include <stdio.h>

// Pointer to a function that implements a partition scheme that counts exchanges
// and the comparisons with variables
typedef int (* partition_scheme_count) (int *, int, int, int *, int *);
typedef int (* partition_scheme) (int *, int, int); // pointer to a regular partition scheme

/**========================================================================
 *!                           Quick sort routines
 *========================================================================**/

// Sort an array with the quicksort algorithm, choosing a partition scheme with `__ps`.
void qs(int *__arr, partition_scheme __ps, int __lo, int __hi);

// Sort an array and count the number of exchanges and comparisons
void qs_count(int *__arr, partition_scheme_count __ps, int __lo, int __hi, int *__ncmp, int *__nech);
/**========================================================================
 *!                          Partition Implementations
 *========================================================================**/
/**============================================
 *!               Partie A
 *=============================================**/
int partition_tp(int *T, int deb, int fin);

int partition_tp_count(int *T, int deb, int fin, int *ncmp, int *nech);

/**============================================
 *!               Partie B
 *=============================================**/
int partition_bis(int *__arr, int __lo, int __hi);

int partition_bis_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);

/**============================================
 *!               Sedgewick
 *=============================================**/
// implemented in Algorithms textbook
int partition_a(int *__arr, int __lo, int __hi);

// implemented in Algorithms textbook
int partition_a_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);

/**============================================
 *!               Lomuto
 *=============================================**/
int partition(int *__arr, int __lo, int __hi);

int partition_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);

/**========================================================================
 *!                           Utility functions
 *========================================================================**/
void inc(int *__a, int __val);

// Swap from two int pointers, making no mention of an array
void swapi(int *__a, int *__b);

// exchange two elements of an array, given their indices
void exchange(int *__arr, int __i, int __j);

// exchange two elements of an array and INCREMENT a variable that represents the
// number of exchanges, passed in as a pointer
void exchange_count(int *__arr, int __i, int __j, int *__nech);

// Print a list in the form;
void print_list(const int *__list, size_t __n);

// Returns an unsorted list that from 0 .. __n - 1 shuffled via fisher-yates
int *unsorted_list(size_t __n);

// return true if the integer array is in ascending order
bool verifier_tri(const int *__list, size_t __n);

// Return a range of numbers from __min to __max (inclusive), with consecutive elements
// separated by __delta
int *range(int __min, int __max, int __delta);
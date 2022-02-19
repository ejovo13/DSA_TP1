#pragma once

#include "ejovo/ejovo_rand.h"

#include <stdlib.h>
#include <stdbool.h>

// Need two simple functions, one that partitions and another that actually performs the quick sort

// Assume that we are sorting integer arrays


// We are going to qsort in-place
void print_list(const int *__list, size_t __n);

void inc(int *__a, int __val);

void exchange(int *__arr, int __i, int __j);

void exchange_count(int *__arr, int __i, int __j, int *__nech);

// Lomuto partition scheme
// test different partition schemes
int partition(int *__arr, int __lo, int __hi);

int partition_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);

// implemented in Algorithms textbook
int partition_a(int *__arr, int __lo, int __hi);

// implemented in Algorithms textbook
int partition_a_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);


void swapi(int *__a, int *__b);

int partition_bis(int *__arr, int __lo, int __hi);

int partition_bis_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);

int partition_tp(int *T, int deb, int fin);

int partition_tp_count(int *T, int deb, int fin, int *ncmp, int *nech);



void quicksort(int *__arr, int __lo, int __hi);

// void quicksort_bis(int *__arr, int __lo, int __hi);

void quicksort_count(int *__arr, int __lo, int __hi, int *__ncmp, int *__nech);

// Returns an unsorted list that from 0 .. __n - 1 shuffled via fisher-yates
int *unsorted_list(size_t __n);



// return true if the integer array is in ascending order
bool verifier_tri(const int *__list, size_t __n);


// on some bossmode shit!!!!

typedef int (* partition_scheme) (int *, int, int);

void qs(int *__arr, partition_scheme __ps, int __lo, int __hi);

typedef int (* partition_scheme_count) (int *, int, int, int *, int *);

void qs_count(int *__arr, partition_scheme_count __ps, int __lo, int __hi, int *__ncmp, int *__nech);



int *range(int __min, int __max, int __delta);
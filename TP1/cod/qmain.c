#include "qsort.h"
#include "ejovo/ejovo_matrix.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define TEST_ARRAY_LENGTH 1000
#define TEST_N_SORTS      10
#define N_MIN             10
#define N_MAX             5000
#define DELTA_N           40
#define M                 100       // The number of arrays to generate at each n value

/**
 * @brief Test the quicksort implementation of various sorting routines
 *
 * The partition schemes that we implement in this TP are Hoare, Lomuto, Sedgewick (adopted from _Algorithms_), and Bis
 *
 */


/**
 * @brief A Partition_scheme stores pointers to functions that implement a counting and non-counting partition scheme
 *
 * This structure allows me to unify the API for the different partition schemes. Instead of having 8 unique entry points,
 * we pair up the `partition` and `partition_count` procedures for each partition scheme.
 *
 *
 */
typedef struct {

    partition_scheme        fn_part; // A pointer to a simple partition function
    partition_scheme_count fn_count; // A pointer to a partition function that count's it's exchanges
    char *                     name; // A String of the name of the scheme

} Partition_scheme;

Partition_scheme Hoare      = {.fn_part = partition_tp,  .fn_count = partition_tp_count,  .name = "Hoare"}; // Used in TP part A
Partition_scheme Lomuto     = {.fn_part = partition,     .fn_count = partition_count,     .name = "Lomuto"};
Partition_scheme Sedgewick  = {.fn_part = partition_a,   .fn_count = partition_a_count,   .name = "Sedgewick"}; // quicksort as implemented in Algorithms
Partition_scheme Bis        = {.fn_part = partition_bis, .fn_count = partition_bis_count, .name = "Bis"};  // quicksort partition scheme for part b

void verify_partition_scheme(Partition_scheme ps); // Assert that a parition scheme used by qs() actually sorts an array
void analyze_complexity(Partition_scheme ps);      // create .csv data of the number of exchanges and comparaisons as a function of n
void example_sort(Partition_scheme ps, size_t n);  // Print an example of the sort


int main() {

    ejovo_seed(); // Shuffle bits of the xoshiro256** prng

    verify_partition_scheme(Hoare);
    verify_partition_scheme(Lomuto);
    verify_partition_scheme(Sedgewick);
    verify_partition_scheme(Bis);
    printf("========================= Testing Complete ===========================\n\n");

    // =============== Begin complexity analysis:=====================
    // analyze_complexity(Hoare);
    // analyze_complexity(Lomuto);
    // analyze_complexity(Sedgewick);
    // analyze_complexity(Bis);
    // printf("Analysis complete\n");

    // =============== Show sorting examples of qs(partition_scheme) ==
    example_sort(Hoare, 30);
    example_sort(Lomuto, 30);
    example_sort(Sedgewick, 30);
    example_sort(Bis, 30);

    return 0;
}

void example_sort(Partition_scheme __ps, size_t __n) {

    int *arr = unsorted_list(__n);
    printf("Unsorted list: ");
    print_list(arr, __n);

    int ncmp = 0;
    int nech = 0;

    qs_count(arr, __ps.fn_count, 0, __n - 1, &ncmp, &nech);

    printf("Partitioninng via: %s\t", __ps.name);
    print_list(arr, __n);

    printf("%d cmp, %d ech:\n\n", ncmp, nech);

    free(arr);
}

void analyze_complexity(Partition_scheme __ps) {

    int ncmp = 0;
    int nech = 0;
    int *N_arr = range(N_MIN, N_MAX, DELTA_N);
    int n_el = ((N_MAX - N_MIN) / DELTA_N) + 1; // number of elements in the range

    Matrix *mean_cmp = Matrix_new(1, n_el); // Create a 1 by nel row vector
    Matrix *mean_ech = Matrix_new(1, n_el); // Create a 1 by nel row vector

    int n = 0;
    int *temp_arr = NULL;
    double avg_cmp = 0;
    double avg_ech = 0;

    printf("Computing averages from n = range(%d, %d, %d)\n", N_MIN, N_MAX, DELTA_N);

    // for each value of n in N_arr,
    for (int i = 0; i < n_el; i++) {

        n = N_arr[i];
        ncmp = 0;
        nech = 0;

        for (int j = 0; j < M; j++) {

            temp_arr = unsorted_list(n); // generate an unsorted list via fisher-yates
            qs_count(temp_arr, __ps.fn_count, 0, n - 1, &ncmp, &nech);
            free(temp_arr);

        }

        Vector_set(mean_cmp, i, (double) ncmp / M); // store the average number of comparisons
        Vector_set(mean_ech, i, (double) nech / M); // store the average number of swaps

    }

    // Create the file_name
    char file_name[50] = "p_";
    strcat(file_name, __ps.name);
    strcat(file_name, ".csv");

    printf("New filename = %s\n", file_name);

    /* Write data to a output file in csv format */
    FILE *fout = fopen(file_name, "w");
    fprintf(fout, "N, avg_cmp, avg_ech\n");

    for (size_t i = 0; i < n_el; i++) {
        fprintf(fout, "%d, %f, %f\n", N_arr[i], Vector_at(mean_cmp, i), Vector_at(mean_ech, i));
    }

    fclose(fout);
}

void verify_partition_scheme(Partition_scheme __ps) {

    int n_tests =  TEST_N_SORTS;
    int len_array = TEST_ARRAY_LENGTH;

    int *list = NULL;

    for (int i = 0; i < n_tests; i++) {

        // shuffle an array via fisher-yates
        list = unsorted_list(len_array);
        qs(list, __ps.fn_part, 0, len_array - 1); // qs using partition scheme "bis"

        assert(verifier_tri(list, len_array));
        free(list);
    }

    // printf("%s partition scheme passed %d sorts of len %d arrays\n", __ps.name, n_tests, len_array);
    printf("Verified %d sort(s) with len: %d arrays | partition scheme: %s\n", n_tests, len_array, __ps.name);
}
/*
 * statistics.c
 *
 * Procedural implementation of basic descriptive statistics
 * (mean, median, mode) on a list of integers.
 *
 * Paradigm: Procedural (C)
 * - Data is stored in a plain int array.
 * - Each statistic is computed by a dedicated function that
 *   operates on that array.
 * - Memory for any dynamically-sized result (the mode list) is
 *   allocated explicitly with malloc() and must be freed by the
 *   caller, illustrating manual memory management in C.
 *
 * Compile:  gcc -Wall -Wextra -std=c11 -o statistics statistics.c
 * Run:      ./statistics
 */

#include <stdio.h>
#include <stdlib.h>

/* ---------- Function prototypes ---------- */
double calculate_mean(const int *data, int n);
double calculate_median(int *data, int n);   /* sorts data in place */
int *calculate_mode(const int *data, int n, int *mode_count, int *highest_freq);
void sort_array(int *data, int n);
void print_array(const int *data, int n);

/* ---------- Mean ---------- */
double calculate_mean(const int *data, int n) {
    long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return (double) sum / n;
}

/* ---------- Simple in-place ascending sort (insertion sort) ----------
 * Chosen for clarity over performance since this is a teaching example;
 * for large n a qsort()-based approach would be preferable.
 */
void sort_array(int *data, int n) {
    for (int i = 1; i < n; i++) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

/* ---------- Median ----------
 * NOTE: This function sorts the caller's array in place. Callers that
 * need to preserve the original order should pass in a copy.
 */
double calculate_median(int *data, int n) {
    sort_array(data, n);

    if (n % 2 == 1) {
        /* Odd count: middle element */
        return (double) data[n / 2];
    } else {
        /* Even count: average the two middle elements */
        int mid1 = data[n / 2 - 1];
        int mid2 = data[n / 2];
        return (mid1 + mid2) / 2.0;
    }
}

/* ---------- Mode ----------
 * Returns a dynamically allocated array containing every value that
 * occurs with the highest frequency (there can be more than one in
 * the case of a tie -- "multi-modal" data).
 *
 * The caller is responsible for free()-ing the returned pointer.
 *
 * *mode_count    is set to the number of modes found.
 * *highest_freq  is set to the frequency of the mode(s).
 */
int *calculate_mode(const int *data, int n, int *mode_count, int *highest_freq) {
    /* Work on a sorted copy so identical values are adjacent; this lets
     * us count frequencies in a single linear pass with O(1) extra
     * space (aside from the result buffer), instead of needing a hash
     * table. */
    int *sorted = malloc(n * sizeof(int));
    if (sorted == NULL) {
        fprintf(stderr, "Error: malloc failed in calculate_mode\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        sorted[i] = data[i];
    }
    sort_array(sorted, n);

    /* First pass: find the highest frequency */
    int best_freq = 1;
    int current_freq = 1;
    for (int i = 1; i < n; i++) {
        if (sorted[i] == sorted[i - 1]) {
            current_freq++;
        } else {
            current_freq = 1;
        }
        if (current_freq > best_freq) {
            best_freq = current_freq;
        }
    }

    /* Second pass: collect every value whose frequency equals best_freq.
     * Worst case every element is its own mode, so allocate n slots. */
    int *modes = malloc(n * sizeof(int));
    if (modes == NULL) {
        fprintf(stderr, "Error: malloc failed in calculate_mode\n");
        free(sorted);
        exit(EXIT_FAILURE);
    }

    int count = 0;
    current_freq = 1;
    for (int i = 1; i <= n; i++) {
        if (i < n && sorted[i] == sorted[i - 1]) {
            current_freq++;
        } else {
            if (current_freq == best_freq) {
                modes[count++] = sorted[i - 1];
            }
            current_freq = 1;
        }
    }

    free(sorted);

    *mode_count = count;
    *highest_freq = best_freq;

    /* Shrink the allocation down to exactly what is needed. realloc to a
     * smaller size is guaranteed not to fail, but we check anyway and
     * fall back to the original block if it somehow does. */
    int *shrunk = realloc(modes, count * sizeof(int));
    return (shrunk != NULL) ? shrunk : modes;
}

/* ---------- Utility: print an array ---------- */
void print_array(const int *data, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", data[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

/* ---------- Demo driver ---------- */
int main(void) {
    int dataset[] = {4, 8, 15, 16, 23, 42, 4, 8, 15, 16, 8};
    int n = (int) (sizeof(dataset) / sizeof(dataset[0]));

    /* Keep an unsorted copy for display, since calculate_median()
     * sorts its input array in place. */
    int *working_copy = malloc(n * sizeof(int));
    if (working_copy == NULL) {
        fprintf(stderr, "Error: malloc failed in main\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        working_copy[i] = dataset[i];
    }

    printf("Dataset: ");
    print_array(dataset, n);
    printf("\n\n");

    double mean = calculate_mean(dataset, n);
    double median = calculate_median(working_copy, n); /* sorts working_copy */

    int mode_count = 0;
    int highest_freq = 0;
    int *modes = calculate_mode(dataset, n, &mode_count, &highest_freq);

    printf("Mean:   %.4f\n", mean);
    printf("Median: %.4f\n", median);

    printf("Mode(s) (frequency = %d): ", highest_freq);
    print_array(modes, mode_count);
    printf("\n");

    /* Manual memory cleanup -- required since C has no garbage collector */
    free(working_copy);
    free(modes);

    return EXIT_SUCCESS;
}

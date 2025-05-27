#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "lms_filter.h"
#include "lms_neon_opt.h"
#include "lms_neon_opt2.h"

#define SIGNAL_LENGTH 80000
#define FILTER_ORDER 32
#define MU 0.01f
#define NUM_TESTS 5

// Generează semnal: sinus + zgomot
void generate_signal(float* input, float* desired, int N) {
    for (int i = 0; i < N; i++) {
        float clean = sinf(2 * 3.14159f * 440 * i / 16000);
        float noise = ((float)rand() / RAND_MAX - 0.5f) * 0.2f;
        input[i] = clean + noise;
        desired[i] = clean;
    }
}

// Eroare medie procentuală absolută
float mean_absolute_percentage_error(float* ref, float* test, int N) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        double denom = fabs(ref[i]);
        if (denom < 1e-6) denom = 1e-6;
        sum += fabs((ref[i] - test[i]) / denom);
    }
    return (float)(100.0 * sum / N);
}

int main() {
    srand(time(NULL));

    float input[SIGNAL_LENGTH];
    float desired[SIGNAL_LENGTH];
    float output_lms[SIGNAL_LENGTH];
    float output_neon[SIGNAL_LENGTH];
    float output_neon_opt2[SIGNAL_LENGTH];

    printf("Comparatie filtre LMS: Clasic vs NEON simplificat vs NEON optimizat 2\n");
    printf("Semnal: %d mostre, Filtru ordine %d\n\n", SIGNAL_LENGTH, FILTER_ORDER);

    for (int test = 1; test <= NUM_TESTS; test++) {
        printf("Test %d/%d:\n", test, NUM_TESTS);

        generate_signal(input, desired, SIGNAL_LENGTH);

        clock_t start = clock();
        lms_filter(input, desired, output_lms, SIGNAL_LENGTH, FILTER_ORDER, MU);
        clock_t end = clock();
        double time_lms = (double)(end - start) / CLOCKS_PER_SEC;

        start = clock();
        lms_neon_opt(input, desired, output_neon, SIGNAL_LENGTH, FILTER_ORDER, MU);
        end = clock();
        double time_neon = (double)(end - start) / CLOCKS_PER_SEC;

        start = clock();
        lms_neon_opt2(input, desired, output_neon_opt2, SIGNAL_LENGTH, FILTER_ORDER, MU);
        end = clock();
        double time_neon_opt2 = (double)(end - start) / CLOCKS_PER_SEC;

        float err_lms = mean_absolute_percentage_error(desired, output_lms, SIGNAL_LENGTH);
        float err_neon = mean_absolute_percentage_error(desired, output_neon, SIGNAL_LENGTH);
        float err_neon_opt2 = mean_absolute_percentage_error(desired, output_neon_opt2, SIGNAL_LENGTH);

        printf("Timp LMS clasic           : %.4f secunde\n", time_lms);
        printf("Timp LMS NEON simplificat : %.4f secunde\n", time_neon);
        printf("Timp LMS NEON optimizat 2 : %.4f secunde\n", time_neon_opt2);
        printf("Eroare MAPE LMS clasic           : %.3f%%\n", err_lms);
        printf("Eroare MAPE LMS NEON simplificat : %.3f%%\n", err_neon);
        printf("Eroare MAPE LMS NEON optimizat 2 : %.3f%%\n", err_neon_opt2);
        printf("---------------------------------------------------\n");
    }

    return 0;
}

#include "lms_filter.h"

void lms_filter(float* input, float* desired, float* output, int N, int order, float mu) {
    float w[order];
    for (int i = 0; i < order; i++)
        w[i] = 0.0f;

    for (int i = order; i < N; i++) {
        float y = 0.0f;
        for (int j = 0; j < order; j++) {
            y += w[j] * input[i - j];
        }
        output[i] = y;
        float e = desired[i] - y;
        for (int j = 0; j < order; j++) {
            w[j] += 2 * mu * e * input[i - j];
        }
    }
}

#include "lms_neon_opt.h"


void lms_neon_opt(float* input, float* desired, float* output, int N, int order, float mu) {
    float w[order];
    for (int i = 0; i < order; i++)
        w[i] = 0.0f;

    for (int i = order; i < N; i++) {
        float y = 0.0f;

        float* w_ptr = w;
        float* x_ptr = &input[i];

        for (int j = 0; j < order; j++) {
            y += w_ptr[j] * x_ptr[-j];
        }

        output[i] = y;
        float e = desired[i] - y;
        float coef = 2.0f * mu * e;

        // Actualizarea coeficienților
        for (int j = 0; j < order; j++) {
            w[j] += coef * input[i - j];
        }
    }
}

/*

#include <arm_neon.h>
#include "lms_neon_opt.h"

void lms_neon_opt(float* input, float* desired, float* output, int N, int order, float mu) {
    float w[order];
    for (int i = 0; i < order; i++)
        w[i] = 0.0f;

    for (int i = order; i < N; i++) {
        float32x4_t acc_vec = vdupq_n_f32(0.0f);
        int j = 0;
        for (; j <= order - 4; j += 4) {
            float32x4_t w_vec = vld1q_f32(&w[j]);
            float32x4_t x_vec = vld1q_f32(&input[i - j]);
            acc_vec = vfmaq_f32(acc_vec, w_vec, x_vec);
        }
        float32_t acc_array[4];
        vst1q_f32(acc_array, acc_vec);
        float y = acc_array[0] + acc_array[1] + acc_array[2] + acc_array[3];

        for (; j < order; j++) {
            y += w[j] * input[i - j];
        }

        output[i] = y;
        float e = desired[i] - y;

        // Actualizare coeficienți scalar (fără NEON)
        for (int k = 0; k < order; k++) {
            w[k] += 2.0f * mu * e * input[i - k];
        }
    }
}*/
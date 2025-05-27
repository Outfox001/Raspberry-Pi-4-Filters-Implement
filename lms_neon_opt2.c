#include "lms_neon_opt2.h"
#include <arm_neon.h>  // NEON intrinsics

void lms_neon_opt2(float* input, float* desired, float* output, int N, int order, float mu) {
    float w[order] __attribute__((aligned(16)));
    for (int i = 0; i < order; i++)
        w[i] = 0.0f;

    float32x4_t v_mu = vmovq_n_f32(2 * mu); // vector cu 4x 2*mu

    for (int i = order; i < N; i++) {
        // Calculez y = w^T * x (produs scalar)
        float32x4_t acc = vmovq_n_f32(0.0f);
        int j;
        for (j = 0; j <= order - 4; j += 4) {
            float32x4_t v_w = vld1q_f32(&w[j]);
            float32x4_t v_x = vld1q_f32(&input[i - j]);
            acc = vmlaq_f32(acc, v_w, v_x);
        }
        // Sumam componentele vectorului acc
        float y = vgetq_lane_f32(acc, 0) + vgetq_lane_f32(acc, 1)
                + vgetq_lane_f32(acc, 2) + vgetq_lane_f32(acc, 3);

        // Restul coeficientilor (daca order nu e divizibil cu 4)
        for (; j < order; j++) {
            y += w[j] * input[i - j];
        }

        output[i] = y;
        float e = desired[i] - y;
        float32x4_t v_e = vmovq_n_f32(e);

        // Actualizare coeficienti w = w + 2 * mu * e * x
        for (j = 0; j <= order - 4; j += 4) {
            float32x4_t v_w = vld1q_f32(&w[j]);
            float32x4_t v_x = vld1q_f32(&input[i - j]);
            float32x4_t v_delta = vmulq_f32(v_mu, vmulq_f32(v_e, v_x));
            v_w = vaddq_f32(v_w, v_delta);
            vst1q_f32(&w[j], v_w);
        }
        for (; j < order; j++) {
            w[j] += 2 * mu * e * input[i - j];
        }
    }
}

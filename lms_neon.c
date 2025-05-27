#include "lms_neon.h"
#include <arm_neon.h>

void lms_neon(float* input, float* desired, float* output, int N, float mu) {
    float32x4_t w = vdupq_n_f32(0.0f);

    for (int i = 0; i < N - 4; i += 4) {
        float32x4_t x_vec = vld1q_f32(&input[i]);
        float32x4_t y_vec = vmulq_f32(w, x_vec);

        float err = vgetq_lane_f32(y_vec, 0) - desired[i];
        float32x4_t e_vec = vdupq_n_f32(err * mu);

        w = vmlaq_f32(w, e_vec, x_vec);

        vst1q_f32(&output[i], y_vec);
    }
}

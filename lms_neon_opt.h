#ifndef LMS_NEON_OPT_H
#define LMS_NEON_OPT_H

void lms_neon_opt(float* input, float* desired, float* output, int N, int order, float mu);

#endif

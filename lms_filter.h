#ifndef LMS_FILTER_H
#define LMS_FILTER_H

void lms_filter(float* input, float* desired, float* output, int N, int order, float mu);

#endif

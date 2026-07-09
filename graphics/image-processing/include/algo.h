#ifndef ALGO_H
#define ALGO_H

void apply_red_filter(void* data, int width, int height, int bytes);
void apply_binarization(void* data, int width, int height, int bytes);
void apply_gaussian_filter(void* data, int width, int height, int bytes);
void apply_sobel_operator(void* data, int width, int height, int bytes);

#endif // ALGO_H
#ifndef ALGO_H
#define ALGO_H

void* apply_red_filter(void* data, int width, int height, int bytes);
void* apply_grayscale(void* data, int width, int height, int bytes);
void* apply_gaussian_filter(void* data, int width, int height);
void apply_sobel_operator(void* data, int width, int height);

#endif // ALGO_H
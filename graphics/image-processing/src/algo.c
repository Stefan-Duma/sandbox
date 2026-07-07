// The process of Canny edge detection algorithm can be broken down to five different steps:
//   Apply Gaussian filter to smooth the image in order to remove the noise
//   Find the intensity gradients of the image
//   Apply gradient magnitude thresholding or lower bound cut-off suppression to get rid of spurious response to edge detection
//   Apply double threshold to determine potential edges
//   Track edge by hysteresis: Finalize the detection of edges by suppressing all the other edges that are weak and not connected to strong edges.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algo.h"

// Gaussian Filter
static const int gf_kernel[5][5] = {
    { 2,  4,  5,  4, 2 },
    { 4,  9, 12,  9, 4 },
    { 5, 12, 15, 12, 5 },
    { 4,  9, 12,  9, 4 },
    { 2,  4,  5,  4, 2 }
};

static const int gf_kernel_sum = 159;
static const int gf_kernel_rows = sizeof(gf_kernel) / sizeof(gf_kernel[0]);
static const int gf_kernel_cols = sizeof(gf_kernel[0]) / sizeof(gf_kernel[0][0]);

// Sobel Operator
static const int so_kernel_x[3][3] = {
    { -1, 0, 1 },
    { -2, 0, 2 },
    { -1, 0, 1 }
};

static const int so_kernel_y[3][3] = {
    { -1, -2, -1 },
    {  0,  0,  0 },
    {  1,  2,  1 }
};

static const int so_kernel_order = 3;

void* apply_red_filter(void* data, int width, int height, int bytes) {
    unsigned char* out = calloc(bytes, sizeof(unsigned char));
    unsigned char* ptr = data;
    
    int px_count = width * height;
    int px_size = bytes / px_count;
    memcpy(out, ptr, bytes);
    for(size_t i = 0; i < bytes / px_size; i++) {
        // R: i * px_size + 0, G: i * px_size + 1, B: i * px_size + 2
        out[i * px_size] = 0;
    }

    return out;
}

// width and height in pixels
void* apply_gaussian_filter(void* data, int width, int height, int bytes) {
    unsigned char* out = calloc(bytes, sizeof(unsigned char));
    unsigned char* ptr = data;
    
    int px_count = width * height;
    int px_size = bytes / px_count;

    int row_radius = gf_kernel_rows / 2;
    int col_radius = gf_kernel_cols / 2;

    int rows = height;
    int cols = width;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            for (int ch = 0; ch < px_size; ch++) {
                int sum = 0;
                for(int k = 0; k < gf_kernel_rows; k++) {
                    for(int l = 0; l < gf_kernel_cols; l++) {
                        int row = i + k - row_radius;
                        int col = j + l - col_radius;
                        
                        if(row >= 0 && col >= 0 && row < rows && col < cols) {
                            int index = (row * cols + col) * px_size + ch;
                            sum += ptr[index] * gf_kernel[k][l];
                        }   
                    }
                }

                int index = (i * cols + j) * px_size + ch;
                out[index] = sum / gf_kernel_sum;
            }
        }
    }
    
    return out;
}
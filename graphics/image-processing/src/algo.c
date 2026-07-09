// The process of Canny edge detection algorithm can be broken down to five different steps:
//   Apply Gaussian filter to smooth the image in order to remove the noise
//   Find the intensity gradients of the image
//   Apply gradient magnitude thresholding or lower bound cut-off suppression to get rid of spurious response to edge detection
//   Apply double threshold to determine potential edges
//   Track edge by hysteresis: Finalize the detection of edges by suppressing all the other edges that are weak and not connected to strong edges.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "algo.h"

#ifndef M_PI
#define M_PI 3.14159f // Good enough
#endif

#define M_RAD2DEG (180.0f/M_PI) // From raylib.h - skips the inclusion of the entire header
#define M_DEG2RAD (M_PI/180.0f) // From raylib.h - skips the inclusion of the entire header

static const float luminance_weights[] = { 0.299, 0.587, 0.114 };
static const int lw_len = 3;
static const int threshhold = 127;

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

void apply_red_filter(void* data, int width, int height, int bytes) {
    unsigned char* ptr = data;
    
    int px_count = width * height;
    int px_size = bytes / px_count;
    
    for(size_t i = 0; i < bytes / px_size; i++) {
        // R: i * px_size + 0, G: i * px_size + 1, B: i * px_size + 2
        ptr[i * px_size + 1] = 0;
        ptr[i * px_size + 2] = 0;
    }
}

void apply_binarization(void* data, int width, int height, int bytes) {
    unsigned char* ptr = data;
    
    int px_count = width * height;
    int px_size = bytes / px_count;

    int rows = height;
    int cols = width;

    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            int sum = 0;
            for(int ch = 0; ch < lw_len; ch++) {
                int index = (i * cols + j) * px_size + ch;
                sum += ptr[index] * luminance_weights[ch];
            }
            
            for(int ch = 0; ch < lw_len; ch++) {
                int index = (i * cols + j) * px_size + ch;
                ptr[index] = sum;
            }
        }
    }
}


// width and height in pixels
void apply_gaussian_filter(void* data, int width, int height, int bytes) {
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
                ptr[index] = sum / gf_kernel_sum;
            }
        }
    }
}

void apply_sobel_operator(void* data, int width, int height, int bytes) {
    signed char* out_x = calloc(bytes, sizeof(signed char));
    signed char* out_y = calloc(bytes, sizeof(signed char));
    unsigned char* ptr = data;
    
    int px_count = width * height;
    int px_size = bytes / px_count;

    int radius = so_kernel_order / 2;

    int rows = height;
    int cols = width;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            for (int ch = 0; ch < px_size; ch++) {
                int sum = 0;
                for(int k = 0; k < so_kernel_order; k++) {
                    for(int l = 0; l < so_kernel_order; l++) {
                        int row = i + k - radius;
                        int col = j + l - radius;
                        
                        if(row >= 0 && col >= 0 && row < rows && col < cols) {
                            int index = (row * cols + col) * px_size + ch;
                            sum += ptr[index] * so_kernel_x[k][l];
                        }   
                    }
                }

                int index = (i * cols + j) * px_size + ch;
                out_x[index] = sum;
            }
        }
    }
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            for (int ch = 0; ch < px_size; ch++) {
                int sum = 0;
                for(int k = 0; k < so_kernel_order; k++) {
                    for(int l = 0; l < so_kernel_order; l++) {
                        int row = i + k - radius;
                        int col = j + l - radius;
                        
                        if(row >= 0 && col >= 0 && row < rows && col < cols) {
                            int index = (row * cols + col) * px_size + ch;
                            sum += ptr[index] * so_kernel_y[k][l];
                        }   
                    }
                }

                int index = (i * cols + j) * px_size + ch;
                out_y[index] = sum;
            }
        }
    }


    float theta = 0.0f;
    float magnitude = 0.0f;
    float negative = 0;
    float positive = 0;
    for(int i = 1; i < rows - 1; i++) {
        for(int j = 1; j < cols - 1; j++) {
            for (int ch = 0; ch < px_size; ch++) {
                int index = (i * cols + j) * px_size + ch;
                int x = out_x[index] * out_x[index];
                int y = out_y[index] * out_y[index];
                
                magnitude = sqrtf(x + y);
                theta = M_RAD2DEG * atan2f(out_y[index], out_x[index]);
                if(theta < 0) theta += 180.0f;
                // rounded to 45
                if(theta >= 45 - 22.5 && theta < 90 - 22.5) {
                    int nw = ((i-1) * cols + j - 1) * px_size + ch;
                    int se = ((i+1) * cols + j + 1) * px_size + ch;
                    
                    int xnw = out_x[nw] * out_x[nw];
                    int ynw = out_y[nw] * out_y[nw];

                    int xse = out_x[se] * out_x[se];
                    int yse = out_y[se] * out_y[se];

                    int magnw = sqrtf(xnw + ynw);
                    int magse = sqrtf(xse + yse);

                    if(magnitude > magnw && magnitude > magse)
                        ptr[index] = magnitude;
                    else
                        ptr[index] = 0; 
                }
                // rounded to 90
                else if(theta >= 90 - 22.5 && theta < 135 - 22.5) {
                    int up = ((i-1) * cols + j) * px_size + ch;
                    int down = ((i+1) * cols + j) * px_size + ch;

                    int xu = out_x[up] * out_x[up];
                    int yu = out_y[up] * out_y[up];

                    int xd = out_x[down] * out_x[down];
                    int yd = out_y[down] * out_y[down];

                    int magu = sqrtf(xu + yu);
                    int magd = sqrtf(xd + yd);

                    if(magnitude > magu && magnitude > magd)
                        ptr[index] = magnitude;
                    else
                        ptr[index] = 0; 
                }
                // rounded to 135
                else if(theta >= 135 - 22.5 && theta < 180 - 22.5) {
                    int ne = ((i-1) * cols + j + 1) * px_size + ch;
                    int sw = ((i+1) * cols + j - 1) * px_size + ch;
                    
                    int xne = out_x[ne] * out_x[ne];
                    int yne = out_y[ne] * out_y[ne];

                    int xsw = out_x[sw] * out_x[sw];
                    int ysw = out_y[sw] * out_y[sw];

                    int magne = sqrtf(xne + yne);
                    int magsw = sqrtf(xsw + ysw);

                    if(magnitude > magne && magnitude > magsw)
                        ptr[index] = magnitude;
                    else
                        ptr[index] = 0; 
                }
                // rounded to 0
                else {
                    int left = (i * cols + j - 1) * px_size + ch;
                    int right = (i * cols + j + 1) * px_size + ch;

                    int xl = out_x[left] * out_x[left];
                    int yl = out_y[left] * out_y[left];

                    int xr = out_x[right] * out_x[right];
                    int yr = out_y[right] * out_y[right];

                    int magl = sqrtf(xl + yl);
                    int magr = sqrtf(xr + yr);

                    if(magnitude > magl && magnitude > magr)
                        ptr[index] = magnitude;
                    else
                        ptr[index] = 0; 
                }
                
            }
        }
    }

    free(out_x);
    free(out_y);
}
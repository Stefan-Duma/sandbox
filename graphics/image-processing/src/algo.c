// The process of Canny edge detection algorithm can be broken down to five different steps:
//   Apply Gaussian filter to smooth the image in order to remove the noise
//   Find the intensity gradients of the image
//   Apply gradient magnitude thresholding or lower bound cut-off suppression to get rid of spurious response to edge detection
//   Apply double threshold to determine potential edges
//   Track edge by hysteresis: Finalize the detection of edges by suppressing all the other edges that are weak and not connected to strong edges.

#include <stdio.h>
#include <stdlib.h>

static const int gf_kernel[5][5] = {
    { 2,  4,  5,  4, 2 },
    { 4,  9, 12,  9, 4 },
    { 5, 12, 15, 12, 5 },
    { 4,  9, 12,  9, 4 },
    { 2,  4,  5,  4, 2 }
};

static const float gf_kernel_sum = 159.0f;
static const int gf_kernel_rows = sizeof(gf_kernel) / sizeof(gf_kernel[0]);
static const int gf_kernel_cols = sizeof(gf_kernel[0]) / sizeof(gf_kernel[0][0]);

// Basic template
int* convolution_of_1D(int* f, size_t len_f, int* g, size_t len_g, int* len_out) {
    *len_out = len_f + len_g - 1;
    int* out = calloc(*len_out, sizeof(int));
    for(int i = 0; i < *len_out; i++) {
        for(int j = 0; j < len_f; j++) {
            int k = i - j;
            if( k >= 0 && k < len_g )
                out[i] += f[j] * g[k];
        }
    }

    return out;
}

int** convolution_of_2D(int** f, size_t rows_f, size_t cols_f, int** g, size_t rows_g, size_t cols_g) {
    // Bad for cache alignment, but easy to implement
    // TODO: Optimize later
    int** out = calloc(rows_f, sizeof(int*));
    for(int i = 0; i < rows_f; i++) {
        out[i] = calloc(cols_f, sizeof(int));
    }

    int row_radius = rows_g / 2;
    int col_radius = cols_g / 2;
    for(int i = 0; i < rows_f; i++) {
        for(int j = 0; j < cols_f; j++) {
            int sum = 0;
            for(int k = 0; k < rows_g; k++) {
                for(int l = 0; l < cols_g; l++) {
                    int row = i + k - row_radius;
                    int col = j + l - col_radius;
                    
                    if(row >= 0 && col >= 0 && row < rows_f && col < cols_f) {    
                        sum += f[row][col] * g[k][l];
                    }
                        
                }
            }
            out[i][j] = sum / gf_kernel_sum;
        }
    }

    return out;
}

int main() {
    int matrix[][12] = {
        {  3,  1,  4,  1,  5,  9,  2,  6,  5,  3,  5,  8 },
        {  9,  7,  9,  3,  2,  3,  8,  4,  6,  2,  6,  4 },
        {  3,  3,  8,  3,  2,  7,  9,  5,  0,  2,  8,  8 },
        {  4,  1,  9,  7,  1,  6,  9,  3,  9,  9,  3,  7 },
        {  5,  1,  0,  5,  8,  2,  0,  9,  7,  4,  9,  4 },
        {  4,  5,  9,  2,  3,  0,  7,  8,  1,  6,  4,  0 },
        {  6,  2,  8,  6,  2,  0,  8,  9,  9,  8,  6,  2 },
        {  8,  0,  3,  4,  8,  2,  5,  3,  4,  2,  1,  1 },
        {  7,  0,  6,  7,  9,  8,  2,  1,  4,  8,  0,  8 },
        {  6,  5,  1,  3,  2,  8,  2,  3,  0,  6,  6,  4 },
        {  7,  0,  9,  3,  8,  4,  4,  6,  0,  9,  5,  5 },
        {  0,  5,  8,  2,  2,  3,  1,  7,  2,  5,  3,  5 }
    };
    
    int matrix_rows = 12;
    int matrix_cols = 12;

    float result[12][12] = { { 0 } };
    
    int f[] = {1, 2, 3};
    int g[] = {4, 5, 6};
    
    int count;
    int* out = convolution_of_1D(f, 3, g, 3, &count);
    for (size_t i = 0; i < count; i++) {
        printf("%d ", out[i]);
    }
    printf("\n");
    

    int* arg1[12];
    int* arg2[5];
    for(int i = 0; i < 12; i++) {
        arg1[i] = matrix[i];
    }
    for(int i = 0; i < 5; i++) {
        arg2[i] = gf_kernel[i];
    }
    int** out2 = convolution_of_2D(arg1, matrix_rows, matrix_cols, arg2, gf_kernel_rows, gf_kernel_cols);

    for(size_t i = 0; i < matrix_rows; i++) {
        for(size_t j = 0; j < matrix_cols; j++) {
            printf("%d ", out2[i][j]);
        }
        printf("\n");
    }

    free(out);
    return 0;
}


#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <span>
#include <ranges>
#include "image_t.h"
#include "utils.h"
#include <cstring>

// ** need to resize destination beforehand
// needs optimization
void multiplyMatrixCol(const int* __restrict matrix, const int* __restrict col_vector, 
    size_t source_row_count, size_t col_vector_dim, int* __restrict destination) {
    // we do matrix * col_vector

    // col_vector_dim = number of columns in each row in matrix and number of elements in each row in matrix
    // source_row_count = number of rows in our matrix
    // output is going to be (source_row_count, 1) shape

    for (size_t y = 0; y < source_row_count; ++y) {
        int matrix_row_sum = 0;
        for (size_t x = 0; x < col_vector_dim; ++x) {
            size_t base_index = y * col_vector_dim + x;
            matrix_row_sum += (col_vector[x] * matrix[base_index]);
            
        }
        destination[y] = matrix_row_sum;
    }
}

// ** need to resize destination beforehand
// needs optization (I think)
int multiplyRowColVector(const int* __restrict row_vector, const int* __restrict col_vector, size_t size) {
    int sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += (row_vector[i] * col_vector[i]);
    }
    return sum;
}

// ** must resize result based on matrix height
// ** you must pass it 3 times for RGB
// optimized
void getColumn(const int* __restrict matrix, int* __restrict result, size_t height, size_t width, size_t channels, size_t col_index) { // jump = original_matrix column count
    if (col_index >= width) [[unlikely]] {
        throw std::invalid_argument("column index is larger than the width");
    }

    size_t jump = width * channels;
    const int* __restrict src = matrix + (col_index * channels);
    int* __restrict dst = result;
    for (size_t r = 0; r < height; ++r) {
        *dst = *src;
        src += jump;
        dst++;
    }
}

// row_count = 0 for first row
// need to get RGB separately
void getRow(const int* __restrict matrix, int* __restrict result, size_t width, size_t channels, size_t row_count) {

    const int* __restrict src = matrix + (width * channels * row_count);
    int* __restrict dst = result;
    for (size_t i = 0; i < width; ++i) {
        *dst = *src;
        src += channels;
        dst++;
    }
}


// multiply two matrices
// needs optimization
void multiplyMatrices(
    const int * __restrict source, 
    const int* __restrict scaling, 
    std::vector<size_t> source_dim,
    std::vector<size_t> scaling_dim, 
    int* __restrict destination) 
{
    // source dimensions expected = (height, width, channels) (what we expect from source_dim)
    // scaling dimensions expected (height, width)
    // destination dimensions = (source_height, scaling_width, channels)
    if (scaling_dim.size() != 2) [[unlikely]] {
        throw std::invalid_argument("the scaling dimension is not right");
    }
    if (source_dim[1] != scaling_dim[0]) [[unlikely]] {
        throw std::invalid_argument("dimensions do not match");
    }
    if (source_dim.size() < 3) [[unlikely]] {
        throw std::invalid_argument("this does not support grayscale yet");
    }
    
    // we do source * scaling

    const size_t scaling_height = scaling_dim[0];
    const size_t scaling_width = scaling_dim[1];
    const size_t source_width = source_dim[1];
    const size_t source_height = source_dim[0];
    const size_t source_channels = source_dim[2];

    for (size_t y = 0; y < source_height; ++y) {
        for (size_t x = 0; x < scaling_width; ++x) {
            for (size_t c = 0; c < source_channels; ++c) {
                int sum = 0;
                for (size_t k = 0; k < source_width; ++k) {
                    int src_val = source[(y * source_width + k) * source_channels + c];
                    int scale_val = scaling[(k * scaling_width + x)];
                    sum += src_val * scale_val;
                }
                destination[(y * scaling_width + x) * source_channels + c] = sum;
            }
        }
    }

    /*
    for (size_t scaling_column_number = 0; scaling_column_number < scaling_width; ++scaling_column_number) {
        std::vector<int> column_buffer(scaling_height);
        getColumn(scaling, column_buffer.data(), scaling_height, scaling_width, 1, scaling_column_number);
        for (size_t y = 0; y < source_height; ++y) {
            for (size_t c = 0; c < source_channels; ++c) { // do it for RGB
                
                std::vector<int> row_buffer(source_width);
                getRow(source + c, row_buffer.data(), source_width, source_channels, y);
                int result = multiplyRowColVector(row_buffer.data(), column_buffer.data(), source_width);
                destination[(y * scaling_width + scaling_column_number) * source_channels + c] = result;
            }
        }
    }
    */
}

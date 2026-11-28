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
#include <random>


void multiplyRowMatrix(const float* __restrict row_vector, const float* __restrict matrix, float* __restrict destination,
     size_t matrix_width, size_t matrix_height) {
    // matrix width == row width since we are multiplying row vector and matrix
    // destination width = matrix height; destination height = 1
    for (size_t y = 0; y < matrix_height; ++y) {
        float sum = 0;
        for (size_t x = 0; x < matrix_width; ++x) {
            sum += matrix[x + y * matrix_width] * row_vector[x];
        }
        destination[y] = sum;
    }
}

void generateRandomValues(std::vector<float>& input_vector) {
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dist(-0.15f, 0.15f);
    for (auto& v : input_vector) v = dist(engine);
}

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

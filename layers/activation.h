#pragma once

#include <algorithm>
#include <cstddef>
#include <cmath>

inline void ReLU(float* __restrict matrix, size_t matrix_width, size_t matrix_height) {
    const size_t total_elements = matrix_width * matrix_height;

    for (size_t i = 0; i < total_elements; ++i) {
        matrix[i] = std::max(0.0f, matrix[i]);
    }
}

inline void cosa(float* __restrict matrix, size_t matrix_width, size_t matrix_height) {
    const size_t total_elements = matrix_width * matrix_height;
    for (size_t i = 0; i < total_elements; ++i) {     
        matrix[i] = 0.7081 * matrix[i] - (0.7071 * std::cos(matrix[i])) + std::cos(matrix[i]);
    }
}

inline void cosa(float* __restrict matrix, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {     
        matrix[i] = 0.7081 * matrix[i] - (0.7071 * std::cos(matrix[i])) + std::cos(matrix[i]);
    }
}
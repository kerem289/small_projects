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

inline std::vector<float> cosa(float* __restrict matrix, size_t matrix_width, size_t matrix_height) {
    
    const size_t total_elements = matrix_width * matrix_height;
    std::vector<float> result(total_elements);
    for (size_t i = 0; i < total_elements; ++i) {     
        result[i] = 0.7081 * matrix[i] - (0.7071 * std::cos(matrix[i])) + std::cos(matrix[i]);
    }
    return result;
}

inline std::vector<float> cosa(float* __restrict matrix, size_t total_elements) {
    std::vector<float> result(total_elements);
    for (size_t i = 0; i < total_elements; ++i) {     
        result[i] = 0.7081 * matrix[i] + 0.2929 * std::cos(matrix[i]);
    }
    return result;
}

inline std::vector<float> deriv_cosa(float* __restrict matrix, size_t total_elements) {
    std::vector<float> result(total_elements);
    for (size_t i = 0; i < total_elements; ++i) {
        result[i] = 0.7081f + (0.7071f * std::sin(matrix[i])) - std::sin(matrix[i]);
    }
    return result;
}

inline std::vector<float> improved_cosa(float* __restrict matrix, size_t total_elements) {
    std::vector<float> result(total_elements);
    for (size_t i = 0; i < total_elements; ++i) {
        result[i] = matrix[i] - 0.1 * std::cos(matrix[i]);
    }
    return result;
}

inline std::vector<float> improved_deriv_cosa(float* __restrict matrix, size_t total_elements) {
    std::vector<float> result(total_elements);
    for (size_t i = 0; i < total_elements; ++i) {
        result[i] = 1 + 0.1 * std::sin(matrix[i]);
    }
    return result;
}

inline std::vector<float> sigmoid(float* __restrict matrix, size_t total_elements) {
    constexpr float E = 2.71828182;
    std::vector<float> result(total_elements);
    for (size_t i = 0; i < total_elements; ++i) {
        result[i] = 1/(1 + std::pow(E, -matrix[i]));
    }
    return result;
}
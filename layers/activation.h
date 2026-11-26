#pragma once

#include <algorithm>
#include <cstddef>
#include <cmath>
#include <cstring>

inline void mReLU(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {
        out[i] = std::max(-0.1f * matrix[i], matrix[i]);
    }
}

inline void deriv_mReLU(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {
        uint32_t raw_bits;
        std::memcpy(&raw_bits, &matrix[i], sizeof(float));
        uint32_t sign = raw_bits >> 31;
        out[i] = 1.0f - (static_cast<float>(sign) * 1.1f);
    }
}

inline void cosa(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {     
        out[i] = 0.7081 * matrix[i] + 0.2929 * std::cos(matrix[i]);
    }
}

inline void deriv_cosa(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {
        out[i] = 0.7081f + (0.7071f * std::sin(matrix[i])) - std::sin(matrix[i]);
    }
}

inline void improved_cosa(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {
        out[i] = matrix[i] - 0.1 * std::cos(matrix[i]);
    }
}

inline void improved_deriv_cosa(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    for (size_t i = 0; i < total_elements; ++i) {
        out[i] = 1 + 0.1 * std::sin(matrix[i]);
    }
}

inline void sigmoid(float* __restrict matrix, float* __restrict out, size_t total_elements) {
    constexpr float E = 2.71828182;
    for (size_t i = 0; i < total_elements; ++i) {
        out[i] = 1/(1 + std::pow(E, -matrix[i]));
    }
}
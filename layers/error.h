#pragma once
#include <cmath>

float MSE(const float* __restrict pred, const float* __restrict actual, size_t size) {
    float error = 0.f;

    for (size_t i = 0; i < size; ++i) {
        error += std::pow((pred[i] - actual[i]), 2);
    }

    return error;
}
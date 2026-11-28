#pragma once
#include <cstddef>

void maxpool(const float* __restrict data, float* __restrict output, const size_t width, const size_t height, const size_t depth,
    const size_t kernel, const size_t stride, const size_t padding);

void averagepool(const float* __restrict data, float* __restrict output, const size_t width, const size_t height, const size_t depth,
    const size_t kernel, const size_t stride, const size_t padding);
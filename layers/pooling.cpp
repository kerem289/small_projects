#include "pooling.h"
#include "../util/inline_helpers.h"
#include <limits>
#include <iostream>
#include <algorithm>

// work on padding later (not now)
void maxpool(const float* __restrict data, float* __restrict output, const size_t input_width, const size_t input_height, const size_t depth,
    const size_t kernel, const size_t stride = 1, const size_t padding = 0) {

    const size_t out_width = (input_width - kernel + 2 * padding)/stride + 1;
    // const size_t out_height = (input_height - kernel)/stride + 1;
    for (size_t y = 0, out_y = 0; y < input_height - kernel + 1; y += stride, ++out_y) {
        for (size_t x = 0, out_x = 0; x < input_width - kernel + 1; x += stride, ++out_x) {
            for (size_t c = 0; c < depth; ++c) {
                float maximum = std::numeric_limits<float>::lowest();
                for (size_t ky = 0; ky < kernel; ++ky) { // kernel y
                    for (size_t kx = 0; kx < kernel; ++kx) { // kernel x
                        const size_t data_idx = ((y + ky) * input_width + (x + kx)) * depth + c;
                        maximum = std::max(maximum, data[data_idx]);
                    }
                }
                const size_t output_idx = (out_y * out_width + out_x) * depth + c;
                output[output_idx] = maximum;
            }
        }
    }
}
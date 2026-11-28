#include "pooling.h"
#include "../util/inline_helpers.h"
#include <limits>
#include <iostream>
#include <algorithm>

void maxpool(const float* __restrict data, float* __restrict output, const size_t input_width, const size_t input_height, const size_t depth,
        const size_t kernel, const size_t stride = 1, const size_t padding = 0) {
    const size_t padded_width = input_width + 2 * padding;
    const size_t padded_height = input_height + 2 * padding;
    const size_t out_width = (padded_width - kernel) / stride + 1;
    const size_t out_height = (padded_height - kernel) / stride + 1;

    for (size_t y = 0, out_y = 0; out_y < out_height; y += stride, ++out_y) {
        for (size_t x = 0, out_x = 0; out_x < out_width; x += stride, ++out_x) {
            float* out_pixel = output + (out_y * out_width + out_x) * depth;
            for (size_t c = 0; c < depth; ++c) {
                out_pixel[c] = std::numeric_limits<float>::lowest();
            }
            for (size_t ky = 0; ky < kernel; ++ky) {
                if (y + ky < padding || y + ky >= input_height + padding) {
                    continue;
                }
                for (size_t kx = 0; kx < kernel; ++kx) {
                    if (x + kx < padding || x + kx >= input_width + padding) {
                        continue;
                    }
                    const float* in_pixel = data + ((y + ky - padding) * input_width + (x + kx - padding)) * depth;
                    for (size_t c = 0; c < depth; ++c) {
                        out_pixel[c] = std::max(out_pixel[c], in_pixel[c]);
                    }
                }
            }
        }
    }
}


void averagepool(const float* __restrict data, float* __restrict output, const size_t input_width, const size_t input_height, const size_t depth,
        const size_t kernel, const size_t stride = 1, const size_t padding = 0) {
    const size_t padded_width = input_width + 2 * padding;
    const size_t padded_height = input_height + 2 * padding;
    const size_t out_width = (padded_width - kernel) / stride + 1;
    const size_t out_height = (padded_height - kernel) / stride + 1;

    for (size_t y = 0, out_y = 0; out_y < out_height; y += stride, ++out_y) {
        for (size_t x = 0, out_x = 0; out_x < out_width; x += stride, ++out_x) {
            float* out_pixel = output + (out_y * out_width + out_x) * depth;
            for (size_t c = 0; c < depth; ++c) {
                out_pixel[c] = 0;
            }
            size_t valid_count = 0;
            for (size_t ky = 0; ky < kernel; ++ky) {
                if (y + ky < padding || y + ky >= input_height + padding) {
                    continue;
                }
                for (size_t kx = 0; kx < kernel; ++kx) {
                    if (x + kx < padding || x + kx >= input_width + padding) {
                        continue;
                    }
                    const float* in_pixel = data + ((y + ky - padding) * input_width + (x + kx - padding)) * depth;
                    ++valid_count;
                    for (size_t c = 0; c < depth; ++c) {
                        out_pixel[c] += in_pixel[c];
                    }
                }
            }
            const size_t denom = count_include_pad ? (kernel * kernel) : valid_count;
            if (denom > 0) {
                const float inv_denom = 1.0f / static_cast<float>(denom);
                for (size_t c = 0; c < depth; ++c) {
                    out_pixel[c] *= inv_denom;
                }
            }
        }
    }
}
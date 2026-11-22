#pragma once

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <random>
#include <utility>
#include "../util/utils.h"

struct Kernel {
    size_t width;
    size_t height;
    std::vector<float> ker;

    Kernel(size_t w, size_t h, std::vector<float> k)
        : width(w), height(h), ker(std::move(k)) {
        if (ker.size() != w * h) {
            throw std::invalid_argument("Kernel data size does not match width * height");
        }
    }

    float getVal(size_t x, size_t y) const {
        if (x >= width || y >= height) {
            throw std::out_of_range("getVal position out of kernel bounds");
        }
        return ker[y * width + x];
    }

    void setPixel(size_t x, size_t y, float value) {
        if (x >= width || y >= height) {
            throw std::out_of_range("setPixel position out of kernel bounds");
        }
        ker[y * width + x] = value;
    }
};

class Conv {
public:
    Kernel kernel;
    
    explicit Conv(Kernel k) : kernel(std::move(k)) {

    }

    template <typename inputType>
    void applyConv(const inputType* __restrict source, std::vector<size_t> source_dim, float* __restrict destination) {

        const float* __restrict ker = kernel.ker.data();
        const size_t H = source_dim[0];
        const size_t W = source_dim[1];
        const size_t C = source_dim[2];
        const size_t outH = H - kernel.height + 1;
        const size_t outW = W - kernel.width + 1;

        destination.resize(outH * outW * C);

        for (size_t y = 0; y < outH; ++y) {
            for (size_t x = 0; x < outW; ++x) {
                for (size_t c = 0; c < C; ++c) {
                    float sum = 0.f;
                    for (size_t ky = 0; ky < kernel.height; ++ky) {
                        for (size_t kx = 0; kx < kernel.width; ++kx) {
                            const size_t kernel_index = ky * kernel.width + kx;
                            const size_t src_index = ((y + ky) * W + (x + kx)) * C + c;
                            sum += static_cast<float>(source[src_index]) * ker[kernel_index];
                        }
                    }
                    destination[(y * outW + x) * C + c] = sum;
                }
            }
        }
    }

    void generateRandomKernel() {
        std::minstd_rand rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(0.f, 1.f);
        for (auto& v : kernel.ker) v = dist(rng);
    }
};

class ReLU {
public:
    void calc(float* __restrict data, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            data[i] = data[i] > 0.f ? data[i] : 0.f;
        }
    }
};
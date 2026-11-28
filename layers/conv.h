#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <random>
#include <utility>
#include "../util/utils.h"
#include "error.h"
#include "activation.h"

// add stride to convolutional layer so it works (do this later not now)
// add batch size (later)
// add pooling (later)
// make model class that can take any type (later)

struct Kernel {
    size_t width;
    size_t height;
    size_t depth;
    std::vector<float> ker;
    float bias;

    Kernel(size_t w, size_t h, size_t d) : width(w), height(h), depth(d) {
        ker.resize(width * height * depth);
        bias = static_cast<float>((std::rand() % 10) / 10.f);
    }
};

class mConv {
public:

    size_t input_width;
    size_t input_height;
    size_t kernel_width;
    size_t kernel_height;
    size_t kernel_depth; // = input_channel
    size_t out_width;
    size_t out_height;
    size_t out_size;
    std::vector<Kernel> kernels;
    std::vector<float> output;
    std::vector<float> last_input_weights;
    std::vector<float> activation_result;
    std::vector<float> activation_derivative;
    std::vector<float> error;
    std::vector<float> delta0;
    std::vector<float> d_ker;
    void (*activation_function)(float* __restrict matrix, float* __restrict out, size_t total_elements);
    void (*activation_func_derivative)(float* __restrict matrix, float* __restrict out, size_t total_elements);

    explicit mConv(size_t kw, size_t kh, size_t kd, size_t iw, size_t ih, size_t amount) : kernel_width(kw), kernel_height(kh), kernel_depth(kd), 
        input_width(iw), input_height(ih) {
        // kd = input_channel
        // @args kernel width, kernel height, kernel dimension, input width, input height, amount of kernel
        // out_width = (input_width - kernel_width + 1)
        // amount = out_dimension
        kernels.reserve(amount);
        for (size_t i = 0; i < amount; ++i) {
            generateKernel();
        }
        out_width = input_width - kernel_width + 1;
        out_height = input_height - kernel_height + 1;
        out_size = amount * out_width * out_height;

        output.resize(out_size);
        activation_result.resize(out_size);
        activation_derivative.resize(out_size);
        error.resize(out_size);
        delta0.resize(input_height * input_width * kernel_depth, 0.f);
        d_ker.resize(kernel_width * kernel_height * kernel_depth, 0.f);
    }

    void generateKernel() {
        Kernel kernel(kernel_width,kernel_height,kernel_depth);
        generateRandomValues(kernel.ker);
        kernels.push_back(kernel);
    }

    void calculate(std::vector<float>& input) {
        // I will optimize this later (less loop). For now, leave it
        last_input_weights = input;
        const float* __restrict source = input.data();
        for (size_t k = 0; k < kernels.size(); ++k) {
            const Kernel& kernel = kernels[k];
            for (size_t y = 0; y < out_height; ++y) {
                for (size_t x = 0; x < out_width; ++x) {
                    float sum = 0.f;
                    for (size_t kernel_y = 0; kernel_y < kernel_height; ++kernel_y) {
                        for (size_t kernel_x = 0; kernel_x < kernel_width; ++kernel_x) {
                            const size_t src_idx = ((y + kernel_y) * input_width + (x + kernel_x)) * kernel_depth;
                            const size_t kernel_idx = (kernel_y * kernel_width + kernel_x) * kernel_depth;
                            for (size_t c = 0; c < kernel_depth; ++c) {
                                sum += source[src_idx + c] * kernel.ker[kernel_idx + c];
                            }
                        }
                    }
                    output[(y * out_width + x) * kernels.size() + k] = sum + kernel.bias;
                }
            }
        }
    }

    void updateLayer(const float* __restrict incoming_error, const float learning_rate) {
        std::fill(delta0.begin(), delta0.end(), 0);
        for (size_t k = 0; k < kernels.size(); ++k) {
            float total_error = 0.f;
            Kernel& kernel = kernels[k];
            std::fill(d_ker.begin(), d_ker.end(), 0.f); // Temporary gradient buffer
            for (size_t y = 0; y < out_height; ++y) {
                for (size_t x = 0; x < out_width; ++x) {
                    const size_t out_idx = (y * out_width + x) * kernels.size() + k;
                    float delta = incoming_error[out_idx] * activation_derivative[out_idx];
                    for (size_t c = 0; c < kernel_depth; ++c) {
                        for (size_t kernel_y = 0; kernel_y < kernel_height; ++kernel_y) {
                            for (size_t kernel_x = 0; kernel_x < kernel_width; ++kernel_x) {
                                const size_t src_idx = ((y + kernel_y) * input_width + (x + kernel_x)) * kernel_depth + c;
                                const size_t kernel_idx = (kernel_y * kernel_width + kernel_x) * kernel_depth + c;
                                delta0[src_idx] += kernel.ker[kernel_idx] * delta;
                                d_ker[kernel_idx] += last_input_weights[src_idx] * delta;
                            }
                        }
                    }
                    total_error += delta;
                }
            }
            kernel.bias -= total_error * learning_rate;
            for (size_t i = 0; i < d_ker.size(); ++i) {
                kernel.ker[i] -= d_ker[i] * learning_rate;
            }
        }
    }

    void updateLastLayer(const float* __restrict error, const float learning_rate) {
        std::fill(delta0.begin(), delta0.end(), 0);
        for (size_t k = 0; k < kernels.size(); ++k) {
            float total_error = 0.f;
            Kernel& kernel = kernels[k];
            std::fill(d_ker.begin(), d_ker.end(), 0.f); // Temporary gradient buffer
            for (size_t y = 0; y < out_height; ++y) {
                for (size_t x = 0; x < out_width; ++x) {
                    const size_t out_idx = (y * out_width + x) * kernels.size() + k;
                    float delta = error[out_idx];
                    
                    for (size_t c = 0; c < kernel_depth; ++c) {
                        for (size_t kernel_y = 0; kernel_y < kernel_height; ++kernel_y) {
                            for (size_t kernel_x = 0; kernel_x < kernel_width; ++kernel_x) {
                                const size_t src_idx = ((y + kernel_y) * input_width + (x + kernel_x)) * kernel_depth + c;
                                const size_t kernel_idx = (kernel_y * kernel_width + kernel_x) * kernel_depth + c;
                                delta0[src_idx] += kernel.ker[kernel_idx] * delta;
                                d_ker[kernel_idx] += last_input_weights[src_idx] * delta;
                            }
                        }
                    }
                    total_error += delta;
                }
            }
            kernel.bias -= total_error * learning_rate;
            for (size_t i = 0; i < d_ker.size(); ++i) {
                kernel.ker[i] -= d_ker[i] * learning_rate;
            }
        }
    }

    void setActivationFunction(void (*func)(float* __restrict matrix, float* __restrict out, size_t total_elements)) {
        activation_function = func;
    }

    void applyActivationFunction() {
        activation_function(output.data(), activation_result.data(), output.size());
    }

    void setActivationDerivative(void (*func)(float* __restrict matrix, float* __restrict out, size_t total_elements)) {
        activation_func_derivative = func;
    }

    void applyActivationDerivative() {
        activation_func_derivative(output.data(), activation_derivative.data(), output.size());
    }
};


class upConv {
public:

};
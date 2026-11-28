#pragma once

#include <vector>
#include "../util/utils.h"
#include <random>
#include "activation.h"
#include <string>
#include "error.h"

class LinearLayer{
public:

    size_t input_width;
    size_t input_height;
    size_t out_width;
    size_t out_height;
    std::vector<float> weights;
    std::vector<float> bias;
    std::vector<float> output;
    std::vector<float> last_input;
    std::vector<float> activation_result;
    std::vector<float> activation_derivative;
    std::vector<float> error;
    std::vector<float> delta0;
    void (*activation_function)(float* __restrict matrix, float* __restrict out, size_t total_elements);
    void (*activation_func_derivative)(float* __restrict matrix, float* __restrict out, size_t total_elements);

    explicit LinearLayer(size_t wi, size_t wo) : input_width(wi), out_width(wo) {
        // output width = weights height
        // input width = weights width
        input_height = 1;
        out_height = 1;
        bias.resize(wo);
        weights.resize(wi * wo); // wo row wi column
        output.resize(wo);
        activation_result.resize(wo);
        activation_derivative.resize(wo);
        error.resize(wo);
        delta0.resize(input_width, 0.f);

        generateRandomValues(weights);
        generateRandomValues(bias);
    }

    void calculate(std::vector<float>& input) {
        last_input = input;
        // std::fill(output.begin(), output.end(), 0.0f);
        multiplyRowMatrix(input.data(), weights.data(), output.data(), input_width, out_width);
        for (size_t i = 0; i < out_width; ++i) {
            output[i] = bias[i] + output[i];
        }
    }

    void updateLayer(const float* __restrict incoming_err, const float learning_rate) {
        std::fill(delta0.begin(), delta0.end(), 0);
        // return_val length = out_width
        for (size_t y = 0; y < out_width; ++y) {
            float delta = incoming_err[y] * activation_derivative[y];
            for (size_t x = 0; x < input_width; ++x) {
                delta0[x] += weights[y * input_width + x] * delta;

                weights[y * input_width + x] -= learning_rate * delta * last_input[x];
            }
            bias[y] -= delta * learning_rate;
        }
    }

    void updateLastLayer(const float* __restrict error, const float learning_rate) {
        std::fill(delta0.begin(), delta0.end(), 0);
        for (size_t y = 0; y < out_width; ++y) {
            float delta = error[y];
            for (size_t x = 0; x < input_width; ++x) {
                delta0[x] += weights[y * input_width + x] * delta; // matrix multiplication
                weights[y * input_width + x] -= error[y] * learning_rate * last_input[x];
            }
            bias[y] -= error[y] * learning_rate;
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
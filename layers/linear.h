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

    explicit LinearLayer(size_t wi, size_t wo) : input_width(wi), out_width(wo) {
        // output width = weights height
        // input width = weights width
        input_height = 1;
        out_height = 1;
        bias.resize(wo);
        weights.resize(wi * wo);
        output.resize(wo);

        generateRandomValues(weights);
        generateRandomValues(bias);
    }

    void calculate(std::vector<float>& input) {
        multiplyRowMatrix(input.data(), weights.data(), output.data(), input_width, out_width);
        for (size_t i = 0; i < out_width; ++i) {
            output[i] = bias[i] + output[i];
        }
    }

};
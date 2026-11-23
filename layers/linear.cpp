#include "linear.h"
#include "activation.h"
#include "error.h"
#include <iostream>

int main() {
    /*
    std::vector<float> si = {0.433f};
    std::vector<float> res = sigmoid(si.data(), 1);
    res[0] = 1.6 - res[0];
    std::cout << res[0] << "\n"; // this is how we scale error
    */


    std::vector<float> train_x = {1.f, 5.f, 102.f, 28.f, 201.f, 88.f, 188.f, 15.f, 150.f, 3.f, 20.f, 500.f, 800.f, 400.f};
    std::vector<float> train_y = {5.f, 13.f, 207.f, 59.f, 405.f, 179.f, 379.f, 33.f, 303.f, 9.f, 43.f, 1003.f, 1603.f, 803.f};

    std::vector<float> test_x = {7.f, 60.f};
    std::vector<float> test_y = {17.f, 123.f};
    
    //std::cout << "0" << "\n";
    float learning_rate = 0.000002;
    LinearLayer layer1(1,1);
    LinearLayer layer2(1,1);
    size_t epochs = 100000;

    for (size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < train_x.size(); ++i) {
            std::vector<float> input_vec{train_x[i]};
            layer1.calculate(input_vec);
            layer1.activation_result = cosa(layer1.output.data(), static_cast<size_t>(layer1.output.size()));
            layer1.activation_derivative = deriv_cosa(layer1.output.data(), static_cast<size_t>(layer1.output.size()));

            layer2.calculate(layer1.output);
            layer2.activation_result = cosa(layer2.output.data(), static_cast<size_t>(layer2.output.size()));
            layer2.activation_derivative = deriv_cosa(layer2.output.data(), static_cast<size_t>(layer2.output.size()));
            std::vector<float> error = merror(layer2.activation_result.data(), &train_y[i], layer2.out_width);
            std::vector<float> layer1_delta = layer2.updateLayer(error.data(), &train_x[i], learning_rate);
            layer1.updateLayer(layer1_delta.data(), &train_x[i], learning_rate);

            if (e % 100000 == 0) {
                std::cout << layer2.activation_result[0] << "-" << train_y[i] << "\n";
            }
        }
    }


    /*
    LinearLayer layer(1,1);
    size_t epochs = 1000000;

    for (size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < train_x.size(); ++i) {
            std::vector<float> input_vec{train_x[i]};
            layer.calculate(input_vec);
            layer.activation_result = cosa(layer.output.data(), static_cast<size_t>(layer.output.size()));
            std::vector<float> error = merror(&layer.activation_result[0], &train_y[i], layer.out_width);
            layer.activation_derivative = deriv_cosa(layer.output.data(), 1);
            layer.updateLayer(error.data(), &train_x[i], learning_rate);
            if (e % 100000 == 0) {
                std::cout << layer.activation_result[0] << "-" << train_y[i] << "\n";
            }
        }
    }
    */

    return 0;
}
#include "linear.h"
#include "activation.h"
#include "error.h"
#include <iostream>
#include "../util/export_layer.h"

// c++ linear.cpp ../util/utils.cpp 
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

    float learning_rate = 0.02;
    LinearLayer layer0(1, 2);
    LinearLayer layer1(2,2);
    LinearLayer layer2(2, 1);
    size_t epochs = 100000;
    float scale = 1000;

    for(size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < train_x.size(); ++i) {
            
            std::vector<float> input_vec{ train_x[i] / scale };
            float target_y = train_y[i] / scale;
            
            layer0.calculate(input_vec);
            layer0.activation_result = improved_cosa(layer0.output.data(), layer0.output.size());
            layer0.activation_derivative = improved_deriv_cosa(layer0.output.data(), layer0.output.size());

            layer1.calculate(layer0.activation_result);
            layer1.activation_result = improved_cosa(layer1.output.data(), layer1.output.size());
            layer1.activation_derivative = improved_deriv_cosa(layer1.output.data(), layer1.output.size());

            layer2.calculate(layer1.activation_result);

            // Backward Pass
            std::vector<float> error = merror(layer2.output.data(), &target_y, layer2.out_width);
            std::vector<float> layer2_delta = layer2.updateLastLayer(error.data(), learning_rate);
            std::vector<float> layer1_delta = layer1.updateLayer(layer2_delta.data(), learning_rate);
            layer0.updateLayer(layer1_delta.data(), learning_rate);
            
            if (e % 10000 == 0 && e != 0) {
                std::cout << layer2.output[0] * scale << "-" << train_y[i] << "\n";
            }
        }
    }

    return 0;
}
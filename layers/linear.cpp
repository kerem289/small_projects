#include "linear.h"
#include "activation.h"
#include <iostream>

int main() {
    /*
    std::vector<float> si = {0.433f};
    std::vector<float> res = sigmoid(si.data(), 1);
    res[0] = 1.6 - res[0];
    std::cout << res[0] << "\n"; // this is how we scale error
    */
    std::vector<float> train_x = {1.f, 5.f, 102.f, 28.f, 201.f, 88.f, 188.f, 15.f, 150.f, 3.f, 20.f};
    std::vector<float> train_y = {5.f, 13.f, 207.f, 59.f, 405.f, 179.f, 379.f, 33.f, 303.f, 9.f, 43.f};

    std::vector<float> test_x = {7.f, 60.f};
    std::vector<float> test_y = {17.f, 123.f};
    
    //std::cout << "0" << "\n";
    float learning_rate = 0.000002;

    LinearLayer layer(1, 1);
    size_t epochs = 1000000;
    for (size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < train_x.size(); ++i) {
            // or just send train_x
            //std::cout << "7" << "\n";
            std::vector<float> input_vec{train_x[i]};
            layer.calculate(input_vec);

            //std::cout << "8" << "\n";
            
            std::vector<float> act_result = cosa(layer.output.data(), static_cast<size_t>(layer.output.size()));
            //std::cout << "9" << "\n";
            float error = train_y[i] - act_result[0];
            //std::cout << "10" << "\n";
            std::vector<float> derivativeActiv = deriv_cosa(layer.output.data(), 1);
            //std::cout << "1" << "\n";
            // std::vector<float> scaling_factor = sigmoid(derivativeActiv.data(), 1);
            //std::cout << "2" << "\n";
            //std::cout << "3" << "\n";
            layer.weights[0] += error * derivativeActiv[0] * learning_rate * train_x[i];
            layer.bias[0] += error * derivativeActiv[0] * learning_rate;
            //std::cout << "4" << "\n";
            //std::cout << "6" << "\n";
            if (e % 100000 == 0) {
                std::cout << act_result[0] << "-" << train_y[i] << "\n";
            }
        }
    }
    std::cout << layer.weights[0] << " " << layer.bias[0] << "\n";
    
    
    /*
    LinearLayer layer(3,5);
    layer.calculate();

    for (int i = 0; i < 5; ++i) {
        std::cout << layer.output[i] << "\n";
    }
    */

    /*
    std::vector<float> v = {0.738461, 2.f, 3.f};
    cosActiv(v.data(), 3, 1);

    for (int i = 0; i < 3; ++i) {
        std::cout << v[i] << "\n";
    }
    */


    return 0;
}
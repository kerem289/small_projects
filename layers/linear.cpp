#include "linear.h"
#include "activation.h"
#include <iostream>

int main() {
    std::vector<float> train_x = {1.f, 5.f, 102.f, 28.f, 201.f, 88.f, 188.f, 15.f};
    std::vector<float> train_y = {5.f, 13.f, 207.f, 59.f, 405.f, 179.f, 379.f, 33.f};

    std::vector<float> test_x = {7.f, 60.f};
    std::vector<float> test_y = {17.f, 123.f};
    
    LinearLayer layer(1, 1);
    size_t epochs = 100;
    for (size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < train_x.size(); ++i) {
            // or just send train_x
            std::vector<float> input_vec{train_x[i]};
            layer.calculate(input_vec);
            cosa(layer.output.data(), static_cast<size_t>(layer.output.size()));
        }
    }
    
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
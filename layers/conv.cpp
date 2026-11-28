#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "memory"
#include "../util/stb_image.h"
#include "../util/stb_image_write.h"
#include "../util/image_t.h"
#include "../util/utils.h"
#include "conv.h"
#include "../models/read_csv.h"
#include "linear.h"
#include "pooling.h"

// c++ -O3 -march=native -ffast-math conv.cpp ../util/utils.cpp ../models/read_csv.cpp pooling.cpp -o main
// this project solves the mnist dataset

// 785
int main() {

    std::unordered_map<std::string_view, float> lookup = DEFAULT_LOOKUP;

    // std::vector<float> scaling = DEFAULT_SCALING;

    const char* filename = "../dataset/mnist_train.csv";
    Dataset dataset = read_file(filename, lookup, 0, 0);

    dataset.cols = 785;
    dataset.rows = 60000;
    std::vector<float> dont = {0};
    divide_data(dataset.data, dont, 255.f, dataset.cols, dataset.rows);
    
    mConv conv0(3, 3, 1, 28, 28, 4);
    mConv conv1(3, 3, 4, 26, 26, 2);
    mConv conv2(3, 3, 2, 24, 24, 1);
    LinearLayer layer1(484, 288);
    LinearLayer layer2(288, 144);
    LinearLayer layer3(144, 10);


    conv0.setActivationFunction(mReLU);
    conv0.setActivationDerivative(deriv_mReLU);

    conv1.setActivationFunction(mReLU);
    conv1.setActivationDerivative(deriv_mReLU);

    conv2.setActivationFunction(mReLU);
    conv2.setActivationDerivative(deriv_mReLU);

    layer1.setActivationFunction(mReLU);
    layer1.setActivationDerivative(deriv_mReLU);

    layer2.setActivationFunction(mReLU);
    layer2.setActivationDerivative(deriv_mReLU);

    size_t epochs = 100;
    float learning_rate = 0.0001;
    std::cout << "starting" << "\n";
    
    for (size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < dataset.rows; ++i) {
            size_t pos = i * dataset.cols; // dataset has 29 columns (first being actual data and the rest being the image)
            float target_y = dataset.data[pos];
            std::vector<float> actual_data(10, 0.f);
            actual_data[target_y] = 1;
            std::vector<float>::const_iterator first = dataset.data.begin() + pos + 1;
            std::vector<float>::const_iterator last = dataset.data.begin() + pos + 785;
            std::vector<float> input_vec(first, last);

            conv0.calculate(input_vec);
            conv0.applyActivationFunction();
            conv0.applyActivationDerivative();

            conv1.calculate(conv0.activation_result);
            conv1.applyActivationFunction();
            conv1.applyActivationDerivative();

            conv2.calculate(conv1.activation_result);
            conv2.applyActivationFunction();
            conv2.applyActivationDerivative();

            layer1.calculate(conv2.activation_result);
            layer1.applyActivationFunction();
            layer1.applyActivationDerivative();

            layer2.calculate(layer1.activation_result);
            layer2.applyActivationFunction();
            layer2.applyActivationDerivative();

            layer3.calculate(layer2.activation_result);

            std::vector<float> error = merror(layer3.output.data(), actual_data.data(), layer3.out_width);
            layer3.updateLastLayer(error.data(), learning_rate);
            layer2.updateLastLayer(layer3.delta0.data(), learning_rate);
            layer1.updateLayer(layer2.delta0.data(), learning_rate);

            conv2.updateLayer(layer1.delta0.data(), learning_rate);
            conv1.updateLayer(conv2.delta0.data(), learning_rate);
            conv0.updateLayer(conv1.delta0.data(), learning_rate);
            
            if (i % 5000 == 0 && e != 0) {
                for (int q = 0; q < 10; ++q) {
                    std::cout << layer3.output[q] << "-" << actual_data[q] << "\n";
                }
                std::cout << "\n";
            }
            if (i % 800 == 0) {
                learning_rate -= 0.0000001;
                if (learning_rate < 0.000001) {
                    learning_rate = 0.000001;
                }
            }
        }
    }

    return 0;
}
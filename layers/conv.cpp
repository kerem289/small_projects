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
    
    mConv conv0(3, 3, 1,  28, 28, 2);
    mConv conv1(3, 3, 2,  26, 26, 4);
    mConv conv2(7, 7, 4,  24, 24, 16);
    LinearLayer layer1(4, 16);
    LinearLayer layer2(16, 32);


    conv0.setActivationFunction(mReLU);
    conv0.setActivationDerivative(deriv_mReLU);

    conv1.setActivationFunction(mReLU);
    conv1.setActivationDerivative(deriv_mReLU);

    conv2.setActivationFunction(improved_cosa);
    conv2.setActivationDerivative(improved_deriv_cosa);

    conv3.setActivationFunction(mReLU);
    conv3.setActivationDerivative(deriv_mReLU);

    conv4.setActivationFunction(mReLU);
    conv4.setActivationDerivative(deriv_mReLU);

    conv5.setActivationFunction(mReLU);
    conv5.setActivationDerivative(deriv_mReLU);

    conv6.setActivationFunction(improved_cosa);    
    conv6.setActivationDerivative(improved_deriv_cosa);

    layer1.setActivationFunction(mReLU);
    layer1.setActivationDerivative(deriv_mReLU);

    layer2.setActivationFunction(mReLU);
    layer2.setActivationDerivative(deriv_mReLU);

    layer3.setActivationFunction(mReLU);
    layer3.setActivationDerivative(deriv_mReLU);

    size_t epochs = 3000;
    float learning_rate = 0.00002;
    std::cout << "starting" << "\n";
    
    for (size_t e = 0; e < epochs; ++e) {
        for (size_t i = 0; i < dataset.rows; ++i) {
            size_t pos = i * dataset.cols; // dataset has 29 columns (first being actual data and the rest being the image)
            float target_y = dataset.data[pos];
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

            conv3.calculate(conv2.activation_result);
            conv3.applyActivationFunction();
            conv3.applyActivationDerivative();

            conv4.calculate(conv3.activation_result);
            conv4.applyActivationFunction();
            conv4.applyActivationDerivative();

            conv5.calculate(conv4.activation_result);
            conv5.applyActivationFunction();
            conv5.applyActivationDerivative();

            conv6.calculate(conv5.activation_result);
            conv6.applyActivationFunction();
            conv6.applyActivationDerivative();

            layer1.calculate(conv6.activation_result);
            layer1.applyActivationFunction();
            layer1.applyActivationDerivative();

            layer2.calculate(layer1.activation_result);
            layer2.applyActivationFunction();
            layer2.applyActivationDerivative();

            layer3.calculate(layer2.activation_result);
            layer3.applyActivationFunction();
            layer3.applyActivationDerivative();

            layer4.calculate(layer3.activation_result);
            std::vector<float> delta1;
            std::vector<float> delta2;
            std::vector<float> error = merror(layer4.output.data(), &target_y, layer4.out_width); // do not make new array every time. Improve this
            layer4.updateLastLayer(error.data(), learning_rate);
            layer3.updateLayer(layer4.delta0.data(), learning_rate);
            layer2.updateLayer(layer3.delta0.data(), learning_rate);
            layer1.updateLayer(layer2.delta0.data(), learning_rate);

            conv6.updateLayer(layer1.delta0.data(), learning_rate);
            conv5.updateLayer(conv6.delta0.data(), learning_rate);
            conv4.updateLayer(conv5.delta0.data(), learning_rate);
            conv3.updateLayer(conv4.delta0.data(), learning_rate);
            conv2.updateLayer(conv3.delta0.data(), learning_rate);
            conv1.updateLayer(conv2.delta0.data(), learning_rate);
            conv0.updateLayer(conv1.delta0.data(), learning_rate);

            if (e % 10 == 0 && e != 0) {
                std::cout << layer4.output[0] << "-" << target_y << "\n";
            }

        }
    }

    return 0;
}
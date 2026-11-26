#include "../layers/linear.h"
#include "../layers/activation.h"
#include "../layers/error.h"
#include "../util/utils.h"
#include <iostream>
#include "../util/export_layer.h"
#include "read_csv.h"

// add weights to each element. ex: 1st element weights more than the 2nd one (scaling)
// c++ house_price_prediction.cpp read_csv.cpp ../util/utils.cpp -o main

int main() {
    std::unordered_map<std::string_view, float> lookup = {
        {std::string_view("Iris-setosa"), 1.f},
        {std::string_view("Iris-virginica"), 2.f},
        {std::string_view("Iris-versicolor"), 3.f}
    };
    const char* filename = "../dataset/Iris.csv";
    Dataset dataset = read_file(filename, lookup, 1, 1);
    dataset.cols = 5;
    dataset.rows = 150;

    // every 5th one is Y

    LinearLayer layer0(4,8);
    LinearLayer layer1(8,4);
    LinearLayer layer2(4,2);
    LinearLayer layer3(2,1);


    
    size_t epochs = 1000000;
    float learning_rate = 0.00002;
    size_t scale = 1;

    std::cout << "starting" << "\n";
    
    for (size_t e = 0; e < epochs; ++e) {
        // std::cout << "in" << "\n";
        for (size_t i = 0; i < dataset.rows; ++i) {
            size_t pos = i * dataset.cols;
            std::vector<float>::const_iterator first = dataset.data.begin() + pos; // make it so that my function accepts O(1)
            std::vector<float>::const_iterator last = dataset.data.begin() + pos + 4;
            std::vector<float> input_vec(first, last);
            float target_y = dataset.data[pos + 4];

            layer0.calculate(input_vec);
            layer0.activation_result = improved_cosa(layer0.output.data(), layer0.output.size());
            layer0.activation_derivative = improved_deriv_cosa(layer0.output.data(), layer0.output.size());

            layer1.calculate(layer0.activation_result);
            layer1.activation_result = improved_cosa(layer1.output.data(), layer1.output.size());
            layer1.activation_derivative = improved_deriv_cosa(layer1.output.data(), layer1.output.size());
            
            layer2.calculate(layer1.activation_result);
            layer2.activation_result = improved_cosa(layer2.output.data(), layer2.output.size());
            layer2.activation_derivative = improved_deriv_cosa(layer2.output.data(), layer2.output.size());

            layer3.calculate(layer2.activation_result);
            std::vector<float> error = merror(layer3.output.data(), &target_y, layer2.out_width);
            std::vector<float> layer3_delta = layer3.updateLastLayer(error.data(), learning_rate);
            std::vector<float> layer2_delta = layer2.updateLayer(layer3_delta.data(), learning_rate);
            std::vector<float> layer1_delta = layer1.updateLayer(layer2_delta.data(), learning_rate);
            layer0.updateLayer(layer1_delta.data(), learning_rate);

            if (e % 1000 == 0 && e != 0) {
                std::cout << layer3.output[0] << "-" << dataset.data[i * 5 + 4] << "\n";
            }

        }
    }

    /*
    for (int i = 0; i < 300; ++i) {
        std::cout << f.data[i] << "\n";
    }
    std::cout << f.data.size() << "\n";
    */
    return 0;
}
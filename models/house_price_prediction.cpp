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
    std::vector<float> scaling = {
        0.f, 0.f, 1.f,
        1.f, 0.f, 1.f,
        2.f, 0.f, 1.f,
        3.f, 0.f, 1.f
    };
    // scaling[0] indicates data index 0 scaling[1] <= dataset index 0 <= scaling[2]
    
    const char* filename = "../dataset/Iris.csv";
    Dataset dataset = read_file(filename, lookup, 1, 1);
    dataset.cols = 5;
    dataset.rows = 150;
    scale_dataset(dataset.data, scaling, dataset.cols, dataset.rows);



    // every 5th one is Y

    LinearLayer layer0(4,8);
    LinearLayer layer1(8,4);
    LinearLayer layer2(4,2);
    LinearLayer layer3(2,1);

    layer0.setActivationFunction(mReLU);
    layer0.setActivationDerivative(deriv_mReLU);
    layer1.setActivationFunction(cosa);
    layer1.setActivationDerivative(deriv_cosa);
    layer2.setActivationFunction(mReLU);
    layer2.setActivationDerivative(deriv_mReLU);
    
    size_t epochs = 100000;
    float learning_rate = 0.00002;

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
            layer0.applyActivationFunction(layer0.output.data(), layer0.output.size());
            layer0.applyActivationDerivative(layer0.output.data(), layer0.output.size());

            layer1.calculate(layer0.activation_result);
            layer1.applyActivationFunction(layer1.output.data(), layer1.output.size());
            layer1.applyActivationDerivative(layer1.output.data(), layer1.output.size());
            
            layer2.calculate(layer1.activation_result);
            layer2.applyActivationFunction(layer2.output.data(), layer2.output.size());
            layer2.applyActivationDerivative(layer2.output.data(), layer2.output.size());

            layer3.calculate(layer2.activation_result);
            std::vector<float> error = merror(layer3.output.data(), &target_y, layer3.out_width);
            std::vector<float> layer3_delta = layer3.updateLastLayer(error.data(), learning_rate);
            std::vector<float> layer2_delta = layer2.updateLayer(layer3_delta.data(), learning_rate);
            std::vector<float> layer1_delta = layer1.updateLayer(layer2_delta.data(), learning_rate);
            layer0.updateLayer(layer1_delta.data(), learning_rate);

            if (e % 10000 == 0 && e != 0) {
                std::cout << layer3.output[0] << "-" << dataset.data[i * 5 + 4] << "\n";
            }
        }
    }

    std::string export_filename = "models.txt";
    export_model<LinearLayer>(export_filename, layer0);
    export_model<LinearLayer>(export_filename, layer1);
    export_model<LinearLayer>(export_filename, layer2);
    export_model<LinearLayer>(export_filename, layer3);
    return 0;

}
#include "../layers/linear.h"
#include "../layers/activation.h"
#include "../layers/error.h"
#include <iostream>
#include "../util/export_layer.h"
#include "read_csv.h"


int main() {
    std::unordered_map<std::string_view, float> lookup = {
        {std::string_view("Iris-setosa"), 1.f},
        {std::string_view("Iris-virginica"), 2.f},
        {std::string_view("Iris-versicolor"), 3.f}
    };
    Dataset data;
    data.cols = 6;
    data.rows = 150;
    const char* filename = "../dataset/Iris.csv";
    auto f = read_file(filename, lookup, 1, 1);

    for (int i = 0; i < 300; ++i) {
        std::cout << f.data[i] << "\n";
    }
    std::cout << f.data.size() << "\n";
    return 0;
}
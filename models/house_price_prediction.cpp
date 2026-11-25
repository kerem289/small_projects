#include "../layers/linear.h"
#include "../layers/activation.h"
#include "../layers/error.h"
#include <iostream>
#include "../util/export_layer.h"
#include "read_csv.h"


int main() {
    std::unordered_map<std::string, float, string_hash, string_equal> lookup = {
        {"Iris-setosa", 1.f},
        {"Iris-virginica", 2.f},
        {"Iris-versicolor", 3.f}
    };
    Dataset data;
    data.cols = 6;
    data.rows = 150;
    const char* filename = "../dataset/Iris.csv";
    auto f = read_file(filename, lookup);

    return 0;
}
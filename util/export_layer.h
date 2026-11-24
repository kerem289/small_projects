#ifndef EXPORT_LAYER_H
#define EXPORT_LAYER_H

#include <string>
#include <iostream>
#include <fstream>


// !! currently designed for linear layer
template<typename dataType>
inline void export_model(std::string file_name, dataType& model) {
    std::ofstream weightsFile(file_name, std::ios_base::app); // with .txt at the end

    if (!weightsFile) {
        std::cerr << "Failed to open or create the file!" << "\n";
        return;
    }

    weightsFile << "weights" << "\n";
    for (size_t y = 0; y < model.out_width; ++y) {
        for (size_t x = 0; x < model.input_width; ++x) {
            if (x != model.input_width - 1) {
                weightsFile << model.weights[y * model.input_width + x] << " ";
            } else {
                weightsFile << model.weights[y * model.input_width + x] << "\n";
            }
        }
    }

    weightsFile << "bias" << "\n";
    for (size_t i = 0; i < model.bias.size(); ++i) {
        if (i != model.bias.size() - 1) {
            weightsFile << model.bias[i] << " ";
        } else {
            weightsFile << model.bias[i] << "\n";
        }
    }

    //weightsFile << "Files can be tricky, but it is fun enough!";
    weightsFile.close();

}


#endif
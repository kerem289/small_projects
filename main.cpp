#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <string>
#include "memory"
#include "util/stb_image.h"
#include "util/stb_image_write.h"
#include "util/image_t.h"
#include "util/utils.h"
// #include <../SFML/Graphics.hpp>

// compile using g++

// ** an example image that resizes image

/*
data[0] is the first pixel's R value.
data[1] is the first pixel's G value.
data[2] is the first pixel's B value.
data[3] is the second pixel's R value.
data[4] is the second pixel's G value.
data[5] is the second pixel's B value.
data[6] is the third pixel's R value.

*/


// #include <GL/glfw.h>

int main(int argc, char** argv) {

    std::vector<int> source = {1,2};
    std::vector<int> scaling = {1,1};

    std::vector<size_t> source_dim = {1, 2, 1}; // (height, width, channels)
    std::vector<size_t> scaling_dim = {2,1}; // (height, width)

    std::vector<int> destination;
    destination.resize(source_dim[0] * scaling_dim[1] * source_dim[2]);

    multiplyMatrices(source.data(), scaling.data(), source_dim, scaling_dim, destination.data());

    for (int i : destination) {
        std::cout << i << "\n";
    }
    
    
    return 0;
}

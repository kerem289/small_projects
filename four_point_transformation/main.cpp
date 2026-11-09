#define STB_IMAGE_IMPLEMENTATION

#include "../util/stb_image.h"

// compile using g++


/*
data[0] is the first pixel's R value.
data[1] is the first pixel's G value.
data[2] is the first pixel's B value.
data[3] is the second pixel's R value.
data[4] is the second pixel's G value.
data[5] is the second pixel's B value.
data[6] is the third pixel's R value.

*/
#include <iostream>
#include <string>
#include "../util/rgb.h"
// #include <SFML/Graphics.hpp>
// #include <GL/glfw.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error: No image path provided.\n";
        std::cerr << "Usage: " << argv[0] << " <image_path>\n";
        return 1;
    }

    const char* image_path = argv[1];
    Image image;

    std::cout << "Attempting to load: " << image_path << "...\n";

    // Load the image (0 means auto-detect original channel count)
    unsigned char* data = stbi_load(image_path, &image.width, &image.height, &image.channels, 0);
    image.pixels = data;

    if (!data) {
        std::cerr << "Failed to load image!\n";
        std::cerr << "Reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Please double-check that the file path is correct and the image is not corrupted.\n";
        return 1;
    }

    // std::cout << (int)data[0] << "\n";

    std::cout << "Dimensions: " << image.width << " x " << image.height << " pixels\n";
    std::cout << "Channels  : " << image.channels << " (";
    if (image.channels == 1) std::cout << "Grayscale";
    else if (image.channels == 3) std::cout << "RGB";
    else if (image.channels == 4) std::cout << "RGBA";
    else std::cout << "Unknown";
    std::cout << ")\n";
    std::cout << "Memory Size: " << (image.width * image.height * image.channels) << " bytes\n";


    stbi_image_free(data);
    std::cout << "Memory freed successfully.\n";

    return 0;
}

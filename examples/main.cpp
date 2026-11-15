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
#include "util/upscale.h"
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
    if (argc < 2) {
        std::cerr << "Error: No image path provided.\n";
        std::cerr << "Usage: " << argv[0] << " <image_path>\n";
        return 1;
    }

    const char* image_path = argv[1];
    Image image;

    std::cout << "Attempting to load: " << image_path << "...\n";

    // Load the image (0 means auto-detect original channel count)
    unsigned char* raw_data = stbi_load(image_path, &image.width, &image.height, &image.channels, 3);
    if (!raw_data) {
        std::cerr << "Failed to load image!\n";
        std::cerr << "Reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Please double-check that the file path is correct and the image is not corrupted.\n";
        return 1;
    }
    
    std::cout << "read the image\n";
    size_t total_bytes = static_cast<size_t>(image.width) * image.height * image.channels;
    image.pixels = std::unique_ptr<uint8_t[]>(new uint8_t[total_bytes]);
    std::memcpy(image.pixels.get(), raw_data, total_bytes); // for now, let it be like this
    stbi_image_free(raw_data);

    if (image.channels == 1) std::cout << "Grayscale\n";
    else if (image.channels == 3) std::cout << "RGB\n";
    else if (image.channels == 4) std::cout << "RGBA\n";
    else std::cout << "Unknown";

    std::cout << "starting resizing now\n";

    // ------ resizing part
    Image resized_image;
    resized_image.width = static_cast<int>(image.width * 2.0f);
    resized_image.height = static_cast<int>(image.height * 2.0f);
    resized_image.channels = 3; // rgb image
    resized_image.pixels = std::unique_ptr<uint8_t[]>(new uint8_t[resized_image.width * resized_image.height * resized_image.channels]);
    std::cout << "starting upscaling\n";
    upscale_rgb(image, resized_image);

    std::cout << "saving the resized image\n";
    stbi_write_png("output_resized.png", resized_image.width, resized_image.height, resized_image.channels, resized_image.pixels.get(), 0);
    std::cout << "Done.\n";

    return 0;
}

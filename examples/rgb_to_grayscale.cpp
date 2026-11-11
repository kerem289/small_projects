#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "memory"
#include "../util/stb_image.h"
#include "../util/stb_image_write.h"
#include "../util/image_t.h"
#include "../util/grayscale.h"
// #include <../SFML/Graphics.hpp>

// ** an example program that converts image into graysacle

// ** usage: g++ rgb_to_grayscale.cpp util/grayscale.cpp -o main
// ** ./main four_point_transformation/img1.jpg

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
    unsigned char* raw_data = stbi_load(image_path, &image.width, &image.height, &image.channels, 0);
    if (!raw_data) {
        
    }
    
    size_t total_bytes = static_cast<size_t>(image.width) * image.height * image.channels;
    image.pixels = std::unique_ptr<uint8_t[]>(new uint8_t[total_bytes]);
    std::memcpy(image.pixels.get(), raw_data, total_bytes); // for now, let it be like this
    stbi_image_free(raw_data);

    // image.pixels = data.get();

    stbi_write_png("output.png", image.width, image.height, image.channels, image.pixels.get(), image.width * image.channels);

    // std::cout << (int)data[0] << "\n";

    std::cout << "Dimensions: " << image.width << " x " << image.height << " pixels\n";
    std::cout << "Channels  : " << image.channels << " (";
    if (image.channels == 1) std::cout << "Grayscale";
    else if (image.channels == 3) std::cout << "RGB";
    else if (image.channels == 4) std::cout << "RGBA";
    else std::cout << "Unknown";
    std::cout << ")\n";
    std::cout << "Memory Size: " << (image.width * image.height * image.channels) << " bytes\n";

    std::cout << "\nattempting to convert image into grayscale\n";
    std::cout << "image channel " << image.channels << "\n";
    Image grayscale_image;
    grayscale_image.width = image.width;
    grayscale_image.height = image.height;
    grayscale_image.channels = 1;
    grayscale_image.pixels = std::unique_ptr<uint8_t[]>(new uint8_t[image.width * image.height]);
    Rgb2Grayscale(image, grayscale_image);

    std::cout << "saving the grayscale image\n";
    stbi_write_png("output_grayscale.png", grayscale_image.width, grayscale_image.height, grayscale_image.channels, grayscale_image.pixels.get(), grayscale_image.width * grayscale_image.channels);


    // stbi_image_free(data);
    std::cout << "Memory freed successfully.\n";

    return 0;
}

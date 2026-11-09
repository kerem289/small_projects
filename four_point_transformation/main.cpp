#define STB_IMAGE_IMPLEMENTATION

#include "../stb_image.h"

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
// #include <SFML/Graphics.hpp>
// #include <GL/glfw.h>

struct RGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;

    RGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
};

struct Image {
    int width;
    int height;
    int channels;
    unsigned char *pixels;

    RGB getPixelRgb(int x, int y) {
        int baseIndex = (y * width + x) * channels;

        return RGB(pixels[baseIndex], pixels[baseIndex + 1], pixels[baseIndex + 2]);
    }

     bool setPixelRgb(int x, int y, RGB color) {
        // add safety checks for different channels
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return false;
        }

        int baseIndex = (y * width + x) * channels;
        pixels[baseIndex] = color.r;
        pixels[baseIndex + 1] = color.g;
        pixels[baseIndex + 2] = color.b;
        if (channels == 4) {
            pixels[baseIndex + 3] = 255; // Default fully opaque
        }
        return true;
    }
};

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

    std::cout << (int)data[0] << "\n";

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

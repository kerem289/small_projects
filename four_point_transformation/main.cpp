#define STB_IMAGE_IMPLEMENTATION

#include "../stb_image.h"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Error: No image path provided.\n";
        std::cerr << "Usage: " << argv[0] << " <image_path>\n";
        return 1;
    }

    const char* image_path = argv[1];
    int width = 0;
    int height = 0;
    int channels = 0;

    std::cout << "Attempting to load: " << image_path << "...\n";

    // Load the image (0 means auto-detect original channel count)
    unsigned char* data = stbi_load(image_path, &width, &height, &channels, 0);

    // Error handling
    if (!data) {
        std::cerr << "Failed to load image!\n";
        std::cerr << "Reason: " << stbi_failure_reason() << "\n";
        std::cerr << "Please double-check that the file path is correct and the image is not corrupted.\n";
        return 1;
    }

    std::cout << "Dimensions: " << width << " x " << height << " pixels\n";
    std::cout << "Channels  : " << channels << " (";
    if (channels == 1) std::cout << "Grayscale";
    else if (channels == 3) std::cout << "RGB";
    else if (channels == 4) std::cout << "RGBA";
    else std::cout << "Unknown";
    std::cout << ")\n";
    std::cout << "Memory Size: " << (width * height * channels) << " bytes\n";


    stbi_image_free(data);
    std::cout << "Memory freed successfully.\n";

    return 0;
}

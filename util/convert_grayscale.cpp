#include <cstdint>
#include <stdexcept>
#include <cmath>
#include "image_t.h"
#include "grayscale.h"


void Rgb2Grayscale(const Image& source, Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width != destination.width || source.height != destination.height)  {
        throw std::invalid_argument("the image dimensions are not the same");
    }

    double RED_COEFF = 0.299;
    double BLUE_COEFF = 0.587;
    double GREEN_COEFF = 0.114;
    destination.channels = 1;

    int total_values = source.width * source.height;
    for (int i = 0; i < (source.width * source.height * source.channels); i += 3) {
        int red = source.pixels[i];
        int green = source.pixels[i+1];
        int blue = source.pixels[i+2];

        double grayscale_value = red * RED_COEFF + green * GREEN_COEFF + blue * BLUE_COEFF;
        uint8_t rounded_grayscale = static_cast<uint8_t>(std::round(grayscale_value));

        destination.pixels[i/3] = rounded_grayscale;
    }
}
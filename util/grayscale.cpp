#include <cstdint>
#include <stdexcept>
#include <cmath>
#include "image_t.h"
#include "grayscale.h"

// a helper cpp class for grayscale images

// converts rgb images into grayscale
void Rgb2Grayscale(const Image& source, Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width != destination.width || source.height != destination.height)  {
        throw std::invalid_argument("the image dimensions are not the same");
    }

    const double RED_COEFF = 0.299;
    const double BLUE_COEFF = 0.114;
    const double GREEN_COEFF = 0.587;
    destination.channels = 1;

    uint8_t *source_ptr = source.pixels.get();
    uint8_t *destination_ptr = destination.pixels.get();

    int total_values = source.width * source.height;
    for (int i = 0; i < (source.width * source.height * source.channels); i += 3) {
        int red = source_ptr[i];
        int green = source_ptr[i+1];
        int blue = source_ptr[i+2];

        double grayscale_value = red * RED_COEFF + green * GREEN_COEFF + blue * BLUE_COEFF;
        uint8_t rounded_grayscale = static_cast<uint8_t>(std::round(grayscale_value));

        destination_ptr[i/3] = rounded_grayscale;
    }
}
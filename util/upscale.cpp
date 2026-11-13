#include <stdexcept>
#include <cmath>
#include <cstdint>
#include "image_t.h"
#include "inline_helpers.h"
#include "upscale.h"

void upscale_rgb(const Image& source, const Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width <= 0 || source.height <= 0 || destination.width <= 0 || destination.height <= 0) {
        throw std::invalid_argument("the image should contain at least one pixel");
    }

    float scale_width = static_cast<float>(destination.width) / source.width;
    float scale_height = static_cast<float>(destination.height) / source.height;

    // int kernel = 3; // automate with kernel later (DO NOT DO IT NOW)

    unsigned char* source_ptr = source.pixels.get();
    unsigned char* destination_ptr = destination.pixels.get();
    int source_size = source.width * source.height * source.channels;

    const float MAIN_POINT_WEIGHT = 0.8f;
    const float OTHER_POINT_WEIGHT = 1.0f - MAIN_POINT_WEIGHT;

    

    for (uint32_t y = 0; y < destination.height; ++y) {
        for (uint32_t x = 0; x < destination.width; ++x) {

            uint32_t source_x = clamp(static_cast<int>(std::round(x / scale_width)), 0, source.width - 1);
            uint32_t source_y = clamp(static_cast<int>(std::round(y / scale_height)), 0, source.height - 1);
            int base_index = (y * destination.width + x) * destination.channels;
            int source_base_index = (source_y * source.width + source_x) * source.channels;

            uint16_t r_sum = 0;
            uint16_t g_sum = 0;
            uint16_t b_sum = 0;
            uint16_t total_values = 8;

            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    if (kx == 0 && ky == 0) continue;

                    int neighbor_x = clamp(source_x + kx, 0, source.width - 1);
                    int neighbor_y = clamp(source_y + ky, 0, source.height - 1);

                    ColorType color = source.getPixelRgb(neighbor_x, neighbor_y);
                    RGB pixel = color.rgb;

                    r_sum += pixel.r;
                    g_sum += pixel.g;
                    b_sum += pixel.b;
                }
            }

            destination_ptr[base_index] = static_cast<uint8_t>(static_cast<float>(r_sum)/total_values * OTHER_POINT_WEIGHT + MAIN_POINT_WEIGHT * source_ptr[source_base_index]);
            destination_ptr[base_index+1] = static_cast<uint8_t>(static_cast<float>(g_sum)/total_values * OTHER_POINT_WEIGHT + MAIN_POINT_WEIGHT * source_ptr[source_base_index+1]);
            destination_ptr[base_index+2] = static_cast<uint8_t>(static_cast<float>(b_sum)/total_values * OTHER_POINT_WEIGHT + MAIN_POINT_WEIGHT * source_ptr[source_base_index+2]);
        }
    }
}
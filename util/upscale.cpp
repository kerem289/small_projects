#include <stdexcept>
#include <cmath>
#include <cstdint>
#include "image_t.h"
#include "inline_helpers.h"
#include "upscale.h"
#include <algorithm>

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define MAIN_POINT_WEIGHT 0.8f
#define OTHER_POINT_WEIGHT (1.f - MAIN_POINT_WEIGHT)

void upscale_rgb(Image& source, Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width <= 0 || source.height <= 0 || destination.width <= 0 || destination.height <= 0) {
        throw std::invalid_argument("the image should contain at least one pixel");
    }

    float scale_width = static_cast<float>(destination.width) / source.width;
    float scale_height = static_cast<float>(destination.height) / source.height;

    // int kernel = 3; // automate with kernel later (DO NOT DO IT NOW)

    unsigned char* source_ptr = source.pixels.data();
    unsigned char* destination_ptr = destination.pixels.data();

    for (int y = 0; y < destination.height; ++y) {
        for (int x = 0; x < destination.width; ++x) {

            int source_x = clamp(static_cast<int>(std::round(x / scale_width)), 0, source.width - 1);
            int source_y = clamp(static_cast<int>(std::round(y / scale_height)), 0, source.height - 1);
            int base_index = (y * destination.width + x) * destination.channels;
            int source_base_index = (source_y * source.width + source_x) * source.channels;

            int r_sum = 0;
            int g_sum = 0;
            int b_sum = 0;
            int total_values = 8;

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

void resize_rgb(Image& source, Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width <= 0 || source.height <= 0 || destination.width <= 0 || destination.height <= 0) {
        throw std::invalid_argument("the image should contain at least one pixel");
    }

    if (source.width == destination.width && source.height == destination.height) {
        destination.pixels = source.pixels;
        return;
    }

    float scale_width = static_cast<float>(destination.width) / source.width;
    float scale_height = static_cast<float>(destination.height) / source.height;

    unsigned char* __restrict destination_ptr = destination.pixels.data();
    unsigned char* __restrict source_ptr = source.pixels.data();

    int scale_wi;
    int scale_hi;
    float scale_hf;
    float scale_wf;

    if (scale_width <= 1) {
        scale_wi = static_cast<int>(std::ceil(1/scale_width));
        scale_wf = scale_width;
    } else {
        scale_wi = 1;
        scale_wf = scale_width;
    }
    // 0.25 -> 4
    if (scale_height <= 1) {
        scale_hi = static_cast<int>(std::ceil(1/scale_height));
        scale_hf = scale_height;
    } else {
        scale_hi = 1;
        scale_hf = scale_height;
    }

    uint16_t total_values = (2 * scale_hi + 1) * (2 * scale_wi + 1) - 1;
    ColorType color;
    for (size_t y = 0; y < destination.height; ++y) {
        for (size_t x = 0; x < destination.width; ++x) {
            uint32_t r_sum = 0;
            uint32_t g_sum = 0;
            uint32_t b_sum = 0;
            int source_x = clamp(static_cast<int>(std::round(x / scale_wf)), 0, source.width - 1);
            int source_y = clamp(static_cast<int>(std::round(y / scale_hf)), 0, source.height - 1);
            int base_index = (y * destination.width + x) * destination.channels;
            int source_base_index = (source_y * source.width + source_x) * source.channels;

            for (int ky = -scale_hi; ky <= scale_hi; ++ky) {
                for (int kx = -scale_wi; kx <= scale_wi; ++kx) {
                    if (kx == 0 && ky == 0) continue;
                    int neighbor_x = clamp(source_x + kx, 0, source.width - 1);
                    int neighbor_y = clamp(source_y + ky, 0, source.height - 1);

                    source.getPixelRgb(neighbor_x, neighbor_y, color);
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


/*
DO NOT TOUCH OR UNDERSTAND THIS PART. THIS PART INSIDE COMMENTS IS NOT WORKING AS INTENDED:
void resize_rgb(Image& source, Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width <= 0 || source.height <= 0 || destination.width <= 0 || destination.height <= 0) {
        throw std::invalid_argument("the image should contain at least one pixel");
    }

    float scale_width = static_cast<float>(destination.width) / source.width;
    float scale_height = static_cast<float>(destination.height) / source.height;

    unsigned char* __restrict destination_ptr = destination.pixels.data();
    unsigned char* __restrict source_ptr = source.pixels.data();

    int scale_wi;
    int scale_hi;
    float scale_hf;
    float scale_wf;

    if (scale_width <= 1) {
        scale_wi = static_cast<int>(std::ceil(1/scale_width));
        scale_wf = scale_width;
    } else {
        scale_wi = static_cast<int>(std::ceil(scale_width));
        scale_wf = scale_width;
    }
    // 0.25 -> 4
    if (scale_hi <= 1) {
        scale_hi = static_cast<int>(std::ceil(1/scale_height));
        scale_hf = scale_height;
    } else {
        scale_hi = static_cast<int>(std::ceil(scale_height));
        scale_hf = scale_height;
    }

    uint16_t total_values = scale_hi * scale_wi - 1;

    for (size_t y = 0; y < destination.height; ++y) {
        for (size_t x = 0; x < destination.width; ++x) {
            uint16_t r_sum = 0;
            uint16_t g_sum = 0;
            uint16_t b_sum = 0;
            uint32_t source_x = clamp(static_cast<int>(std::round(x / scale_wf)), 0, source.width - 1);
            uint32_t source_y = clamp(static_cast<int>(std::round(y / scale_hf)), 0, source.height - 1);
            int base_index = (y * destination.width + x) * destination.channels;
            int source_base_index = (source_y * source.width + source_x) * source.channels;

            for (int ky = -scale_hi; ky <= scale_hi; ++ky) {
                for (int kx = -scale_wi; kx <= scale_wi; ++kx) {
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
*/


/*
DO NOT TOUCH OR UNDERSTAND THIS PART. THIS PART INSIDE COMMENTS IS NOT WORKING AS INTENDED:
(FEATURE EXTRACTTION????)

void resize_rgb(Image& source, Image& destination) {
    if (source.channels != 3) {
        throw std::invalid_argument("image is not rgb");
    }

    if (source.width <= 0 || source.height <= 0 || destination.width <= 0 || destination.height <= 0) {
        throw std::invalid_argument("the image should contain at least one pixel");
    }

    if (source.width == destination.width && source.height == destination.height) {
        destination.pixels = source.pixels;
        return;
    }

    float scale_width = static_cast<float>(destination.width) / source.width;
    float scale_height = static_cast<float>(destination.height) / source.height;

    unsigned char* __restrict destination_ptr = destination.pixels.data();
    unsigned char* __restrict source_ptr = source.pixels.data();

    int scale_wi;
    int scale_hi;
    float scale_hf;
    float scale_wf;

    if (scale_width <= 1) {
        scale_wi = static_cast<int>(std::ceil(1/scale_width));
        scale_wf = scale_width;
    } else {
        scale_wi = static_cast<int>(std::ceil(scale_width));
        scale_wf = scale_width;
    }
    // 0.25 -> 4
    if (scale_height <= 1) {
        scale_hi = static_cast<int>(std::ceil(1/scale_height));
        scale_hf = scale_height;
    } else {
        scale_hi = static_cast<int>(std::ceil(scale_height));
        scale_hf = scale_height;
    }

    uint16_t total_values = (2 * scale_hi + 1) * (2 * scale_wi + 1) - 1;

    for (size_t y = 0; y < destination.height; ++y) {
        for (size_t x = 0; x < destination.width; ++x) {
            uint32_t r_sum = 0;
            uint32_t g_sum = 0;
            uint32_t b_sum = 0;
            int source_x = clamp(static_cast<int>(std::round(x / scale_wf)), 0, source.width - 1);
            int source_y = clamp(static_cast<int>(std::round(y / scale_hf)), 0, source.height - 1);
            int base_index = (y * destination.width + x) * destination.channels;
            int source_base_index = (source_y * source.width + source_x) * source.channels;

            for (int ky = -scale_hi; ky <= scale_hi; ++ky) {
                for (int kx = -scale_wi; kx <= scale_wi; ++kx) {
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
*/
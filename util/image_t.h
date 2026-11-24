#pragma once
#include <iostream>
#include <cstdint>
#include "memory"
#include <vector>

struct Grayscale {
    uint8_t value;

    Grayscale() : value(0) {}
    Grayscale(uint8_t val) : value(val) {}
};

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    RGB() : r(0), g(0), b(0) {}
    RGB(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
};

struct ColorType {
    int channels;

    union {
        RGB rgb;
        Grayscale grayscale;
    };

    ColorType(int ch, Grayscale g) : channels(ch) {
        grayscale = g;
    }
    ColorType(int ch, RGB r) : channels(ch) {
        rgb = r;
    }
};

// !! convert these into size_t and test if its fast
struct Image {
    size_t width;
    size_t height;
    size_t channels;
    std::vector<uint8_t> pixels;

    ColorType getPixelRgb(size_t x, size_t y) const {
        size_t baseIndex = (y * width + x) * channels;

        if (channels == 1) {
            return ColorType{1, Grayscale(pixels[baseIndex])};
        }
        else if(channels == 3) {
            return ColorType{3, RGB(pixels[baseIndex], pixels[baseIndex + 1], pixels[baseIndex + 2])};

        }

        return ColorType{3, RGB(pixels[baseIndex], pixels[baseIndex + 1], pixels[baseIndex + 2])};
    }

     void setPixelRgb(size_t x, size_t y, RGB color) {
        // add safety checks for different channels
        if (x >= width || y >= height) {
            return;
        }
        size_t baseIndex = (y * width + x) * channels;
        if (channels == 1) {
            pixels[baseIndex] = static_cast<uint8_t>(0.299f * color.r + 0.587f * color.g + 0.114f * color.b);
        } else if (channels == 3) {
            pixels[baseIndex] = color.r;
            pixels[baseIndex + 1] = color.g;
            pixels[baseIndex + 2] = color.b;
        } else if (channels == 4) {
            pixels[baseIndex] = color.r;
            pixels[baseIndex + 1] = color.g;
            pixels[baseIndex + 2] = color.b;
            pixels[baseIndex + 3] = 255; // Default fully opaque
        } else {
            pixels[baseIndex] = color.r;
            pixels[baseIndex + 1] = color.g;
            pixels[baseIndex + 2] = color.b;

        }
        return;
    }
};

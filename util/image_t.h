#include <iostream>
#include <cstdint>
#include "memory"
#pragma once

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

struct Image {
    int width;
    int height;
    int channels;
    std::unique_ptr<uint8_t[]> pixels; // must use get function to get the pointer

    ColorType getPixelRgb(int x, int y) {
        int baseIndex = (y * width + x) * channels;

        if (channels == 1) {
            return ColorType{1, Grayscale(pixels[baseIndex])};
        }
        else if(channels == 3) {
            return ColorType{3, RGB(pixels[baseIndex], pixels[baseIndex + 1], pixels[baseIndex + 2])};
            
        }

        return ColorType{3, RGB(pixels[baseIndex], pixels[baseIndex + 1], pixels[baseIndex + 2])};
    }

     bool setPixelRgb(int x, int y, RGB color) {
        // add safety checks for different channels
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return false;
        }
        int baseIndex = (y * width + x) * channels;
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
        return true;
    }
};

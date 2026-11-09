#include <iostream>

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
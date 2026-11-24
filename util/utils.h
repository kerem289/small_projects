#pragma once
#include <vector>
#include <stdexcept>
#include <cstddef>

void multiplyRowMatrix(const float* __restrict row_vector, const float* __restrict matrix, float* __restrict destination,
     size_t matrix_width, size_t matrix_height);

void generateRandomValues(std::vector<float>& input_vector);

void multiplyMatrixCol(const int* __restrict matrix, const int* __restrict col_vector, 
    size_t source_row_count, size_t col_vector_dim, int* __restrict destination);

int multiplyRowColVector(const int* __restrict row_vector, const int* __restrict col_vector, size_t size);

void getColumn(const int* __restrict matrix, int* __restrict result, size_t height, size_t width, size_t channels, size_t col_index);

void getRow(const int* __restrict matrix, int* __restrict result, size_t width, size_t channels, size_t row_count);

template <typename inputType, typename dataType>
void multiplyMatrices(
    const inputType * __restrict source, 
    const dataType* __restrict scaling, 
    std::vector<size_t> source_dim,
    std::vector<size_t> scaling_dim, 
    dataType* __restrict destination) 
{
    // source dimensions expected = (height, width, channels) (what we expect from source_dim)
    // scaling dimensions expected (height, width)
    // destination dimensions = (source_height, scaling_width, channels)
    if (scaling_dim.size() != 2) [[unlikely]] {
        throw std::invalid_argument("the scaling dimension is not right");
    }
    if (source_dim.size() < 3) [[unlikely]] {
        throw std::invalid_argument("this does not support grayscale yet");
    }
    if (source_dim[1] != scaling_dim[0]) [[unlikely]] {
        throw std::invalid_argument("dimensions do not match");
    }
    
    
    // we do source * scaling

    const size_t scaling_height = scaling_dim[0];
    const size_t scaling_width = scaling_dim[1];
    const size_t source_width = source_dim[1];
    const size_t source_height = source_dim[0];
    const size_t source_channels = source_dim[2];

    for (size_t y = 0; y < source_height; ++y) {
        for (size_t x = 0; x < scaling_width; ++x) {
            for (size_t c = 0; c < source_channels; ++c) {
                dataType sum = 0;
                for (size_t k = 0; k < source_width; ++k) {
                    inputType src_val = source[(y * source_width + k) * source_channels + c];
                    dataType scale_val = scaling[(k * scaling_width + x)];
                    sum += src_val * scale_val;
                }
                destination[(y * scaling_width + x) * source_channels + c] = sum;
            }
        }
    }
}
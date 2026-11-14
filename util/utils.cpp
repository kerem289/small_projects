#include <iostream>
#include <vector>
#include <stdexcept>
#include <iterator>
#include <cstddef>
#include <span>
#include <ranges>

// !! adjust it to vector
void multiplyMatrixCol(const int* __restrict matrix, auto* col_vector, 
    size_t source_row_count, size_t col_vector_dim, int* __restrict destination) {
    // we do matrix * col_vector

    // col_vector_dim = number of columns in each row in matrix and number of elements in each row in matrix
    // source_row_count = number of rows in our matrix
    // output is going to be (source_row_count, 1) shape

    for (size_t y = 0; y < source_row_count; ++y) {
        int matrix_row_sum = 0;
        for (size_t x = 0; x < col_vector_dim; ++x) {
            size_t base_index = y * col_vector_dim + x;
            matrix_row_sum += (col_vector[x] * matrix[base_index]);
            
        }
        destination[y] = matrix_row_sum;
    }
}


void multiplyRowColVector(const int* __restrict row_vector, auto* __restrict col_vector, size_t size, int* __restrict destination) {
    int sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += (row_vector[i] * col_vector[i]);
    }
    *destination = sum;
}

std::vector<int> getColumn(std::vector<int> matrix, size_t jump) { // jump = original_matrix column count
    auto column_view = matrix | std::views::stride(count);
    return std::ranges::to<std::vector<int>>(column_view);
}


// multiply two matrices
void multiplyMatrices(const int * __restrict source, int* __restrict scaling, 
        std::span<size_t, std::dynamic_extent> source_dim, std::span<size_t, std::dynamic_extent> scaling_dim, 
        int* __restrict destination) {
    // source dimensions expected = (height, width, channels) (what we expect from source_dim)
    // scaling dimensions expected (height, width)
    // elements are stored as 
    if (std::size(scaling_dim) != 2) {
        throw std::invalid_argument("the scaling dimension is not right");
    }
    if (source_dim[0] != scaling_dim[1]) {
        throw std::invalid_argument("dimensions do not match");
    }
    
    // we do source * scaling

    for (size_t y = 0; y < surce_dim[0]; ++y) {
        int* image_column = getColumn(matrix).data();
        multiplyMatrixCol(source[y], )
    }

    if ()
}

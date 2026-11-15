#pragma once

void multiplyMatrixCol(const int* __restrict matrix, const int* __restrict col_vector, 
    size_t source_row_count, size_t col_vector_dim, int* __restrict destination);

int multiplyRowColVector(const int* __restrict row_vector, const int* __restrict col_vector, size_t size);

void getColumn(const int* __restrict matrix, int* __restrict result, size_t height, size_t width, size_t channels, size_t col_index);

void getRow(const int* __restrict matrix, int* __restrict result, size_t width, size_t channels, size_t row_count);

void multiplyMatrices(const int * __restrict source, const int* __restrict scaling, std::vector<size_t> source_dim, std::vector<size_t> scaling_dim, int* __restrict destination);


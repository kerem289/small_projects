#ifndef READ_CSV_H
#define READ_CSV_H
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

struct Dataset {
    std::vector<float> data; // Flat contiguous array (Rows * Cols)
    size_t rows = 0;
    size_t cols = 0;
};

Dataset read_file(const char* filename, std::unordered_map<std::string_view, float>& lookup, bool skip_header, bool skip_first_col);

#endif
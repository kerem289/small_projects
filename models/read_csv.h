#ifndef READ_CSV_H
#define READ_CSV_H
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <functional>

#define DEFAULT_LOOKUP {{"hohohoho", 1.f}}
#define DEFAULT_SCALING {}

struct Dataset {
    std::vector<float> data; // Flat contiguous array (Rows * Cols)
    size_t rows = 0;
    size_t cols = 0;
};

Dataset read_file(const char* filename, std::unordered_map<std::string_view, float>& lookup, bool skip_header, bool skip_first_col);
void scale_dataset(std::vector<float>& dataset, std::vector<float>& scaling, size_t cols, size_t rows);

#endif
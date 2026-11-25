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

struct string_hash {
    using is_transparent = void;

    size_t operator()(std::string_view sv) const {
        return std::hash<std::string_view>{}(sv);
    }
    size_t operator()(const std::string& s) const {
        return std::hash<std::string>{}(s);
    }
    size_t operator()(const char* s) const {
        return std::hash<std::string_view>{}(s);
    }
};

struct string_equal {
    using is_transparent = void;

    bool operator()(std::string_view lhs, std::string_view rhs) const {
        return lhs == rhs;
    }
};

Dataset read_file(
    const char* filename,
    std::unordered_map<std::string, float, string_hash, string_equal> lookup
);

#endif
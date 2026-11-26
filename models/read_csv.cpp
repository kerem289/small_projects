#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <charconv>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include "read_csv.h"
#include <limits>


Dataset read_file(const char* filename, std::unordered_map<std::string_view, float>& lookup, bool skip_header = false, bool skip_first_col = false) {
    Dataset data;
    constexpr size_t FLOAT_SIZE = sizeof(float);

    
    int fd = open(filename, O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    size_t length = sb.st_size; // get total bytes
    char* addr = static_cast<char*>(mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0)); // memory map dataset
    if (addr == MAP_FAILED) {
        close(fd);
        throw std::invalid_argument("could not map the file");
        return data;
    }

    std::string_view file_view(addr, length);
    data.data.reserve(length/4);
    size_t cursor = 0;

    if (skip_header && cursor < length) { // skip header
        size_t first_newline = file_view.find('\n', cursor);
        cursor = (first_newline == std::string_view::npos) ? length : first_newline + 1;
    }

    while (cursor < length) {
        size_t next_newline = file_view.find("\n", cursor);
        if (next_newline == std::string_view::npos) {
            next_newline = length;
        }
        std::string_view line = file_view.substr(cursor, next_newline - cursor);
        if (!line.empty() && line.back() == '\r') { // if have trailing \r
            line.remove_suffix(1);
        }
        if (line.empty()) {
            cursor = next_newline + 1;
            continue;
        }
        size_t line_cursor = 0;

        if (skip_first_col) {
            size_t first_comma = line.find(",", line_cursor);
            if (first_comma == std::string_view::npos) {
                cursor = next_newline + 1; // empty row
            }
            line_cursor = first_comma + 1;
        }

        while (line_cursor < line.length()) {
            size_t next_comma = line.find(",", line_cursor);
            if (next_comma == std::string_view::npos) {
                next_comma = line.length();
            }
            std::string_view cell = line.substr(line_cursor, next_comma - line_cursor);
            auto it = lookup.find(cell);
            if (it != lookup.end()){
                data.data.push_back(it->second);
            }
            else {
                float val;
                auto [ptr, ec] = std::from_chars(cell.data(), cell.data() + cell.length(), val);
                if (ec == std::errc()) {
                    data.data.push_back(static_cast<float>(val));
                }
            }
            line_cursor = next_comma + 1;
        }
        data.rows++;
        cursor = next_newline + 1;
    }
    munmap(addr, length);
    close(fd);
    return data;
}

void scale_dataset(std::vector<float>& dataset, std::vector<float>& scaling, size_t cols, size_t rows) {
    if (!scaling.empty()) {
        size_t total_size = cols * rows;
        size_t count = scaling.size() / 3;
        std::vector<float> max_values(count, std::numeric_limits<float>::lowest());
        std::vector<float> min_values(count, std::numeric_limits<float>::max());
        if (rows == 0 || cols == 0 || dataset.size() != cols * rows) {
        throw std::invalid_argument("Dataset dimensions do not match cols * rows");
        }
        if (scaling.size() % 3 != 0) {
            throw std::invalid_argument("Scaling vector size must be a multiple of 3");
        }
        for (size_t r = 0; r < rows; ++r) {
            for (size_t i = 0; i < count; ++i) {
                size_t column = static_cast<size_t>(scaling[i*3]);
                max_values[i] = std::max(max_values[i], dataset[r * cols + column]);
                min_values[i] = std::min(min_values[i], dataset[r * cols + column]);
            }
        }

        for (size_t i = 0; i < count; ++i) {
        if (std::abs(max_values[i] - min_values[i]) <= 1e-7f) {
            throw std::invalid_argument("All values are the same for a scaled column");
            }
        }

        for (size_t r = 0; r < rows; ++r) {
            for (size_t i = 0; i < count; ++i) {
                size_t column = static_cast<size_t>(scaling[i*3]);
                float val = dataset[r * cols + column];
                dataset[r * cols + column] = scaling[i*3 + 1] + ((val - min_values[i])/(max_values[i] - min_values[i]) * (scaling[i*3+2] - scaling[i*3 + 1]));
            }
        }
    }
}
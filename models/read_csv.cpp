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

Dataset read_file(const char* filename, std::unordered_map<std::string, float, string_hash, string_equal> lookup) {
    Dataset data;
    int fd = open(filename, O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    size_t length = sb.st_size; // get total bytes
    char* addr = static_cast<char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0));
    if (addr == MAP_FAILED) {
        close(fd);
        throw std::invalid_argument("could not map the file");
        return data;
    }
    std::string_view file_view(addr, length);
    data.data.reserve(length/10);
    size_t cursor = 0;
    while (cursor < length) {
        size_t next_newline = file_view.find("\n", cursor);
        if (next_newline == std::string_view::npos) {
            next_newline = length;
        }
        std::string_view line = file_view.substr(cursor, next_newline - cursor);
        size_t line_cursor = 0;
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
                    data.data.push_back(val);
                }
            }
            line_cursor = next_comma + 1;
        }
        data.rows++;
        cursor += next_newline + 1;
    }
    munmap(addr, length);
    close(fd);
    return data;
}
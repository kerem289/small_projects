#pragma once
#include <iostream>

inline int clamp(int val, int min_val, int max_val) {
    return std::max(min_val, std::min(val, max_val));
}
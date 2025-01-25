#ifndef PRINTER_HPP
#define PRINTER_HPP

#include <iostream>
#include <vector>
#include <array>

// Template for std::vector
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i + 1 < vec.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

// Template for std::array
template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr) {
    os << "[";
    for (size_t i = 0; i < N; ++i) {
        os << arr[i];
        if (i + 1 < N) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}
#endif // PRINTER_HPP
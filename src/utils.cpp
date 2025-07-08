#include "../include/utils.h" 
#include <stdexcept>

#define EPSILON 0.001

std::vector<double> linspace_double(double start, double end, double num) {
    if (num < 0) {
        throw std::invalid_argument("linspace_double: num must be >= 0");
    }

    std::vector<double> result;
    if (num == 0) return result;
    if (num == 1) {
        result.push_back(start);
        return result;
    }

    // Avoid floating-point accumulation errors by computing each term independently
    double step = (end - start) / (num - 1);
    for (int i = 0; i < num; ++i) {
        result.push_back(start + i * step);
    }

    return result;
}

bool double_equals(double a, double b)
{
    return std::abs(a - b) < EPSILON;
}

#include <vector>

/**
 * @brief Generates a linearly spaced sequence of doubles between [start, end].
 * @param start The starting value of the sequence (inclusive).
 * @param end The ending value of the sequence (inclusive).
 * @param num The number of points to generate. Must be non-negative.
 *             - If `num == 0`, returns an empty vector.
 *             - If `num == 1`, returns a vector containing `start`.
 * @return std::vector<double> A vector of `num` evenly spaced values.
 * @throws std::invalid_argument If `num` is negative.
 * @note The endpoint `end` is guaranteed to be included exactly, avoiding floating-point accumulation errors.
 * @example
 *   std::vector<double> seq = linspace_double(0.0, 1.0, 5);
 *   // seq = [0.0, 0.25, 0.5, 0.75, 1.0]
 */
std::vector<double> linspace_double(double start, double end, double num);

/**
 *  @brief Checks if two doubles are equal within a given epsilon.
 *  @param a The first double to compare.
 *  @param b The second double to compare.
 *  @param epsilon The maximum difference between a and b to be considered equal.
 *  @return bool True if the absolute difference between a and b is less than or equal to epsilon, false otherwise.
 */
bool double_equals(double a, double b);

void map_to_color(int numero, int* map_r, int* map_g, int* map_b, int len, int* r, int* g, int* b);

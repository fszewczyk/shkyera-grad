#pragma once

#include <random>
#include <sstream>
#include <string>

namespace st::utils {

std::random_device RANDOM_DEVICE;
std::mt19937 RANDOM_GENERATOR(RANDOM_DEVICE());

template <typename T> T convert(const std::string &str) {
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}

template <typename T> T randomValue(T low, T high) {
    if (std::is_floating_point<T>::value) {
        std::uniform_real_distribution<> distribution(low, high);
        return distribution(RANDOM_GENERATOR);
    } else if (std::is_arithmetic<T>::value) {
        std::uniform_int_distribution<> distribution(low, high);
        return distribution(RANDOM_GENERATOR);
    } else {
        return 0;
    }
}

template <typename T> void shuffle(std::vector<T> &v) { std::shuffle(std::begin(v), std::end(v), RANDOM_GENERATOR); }

} // namespace st::utils

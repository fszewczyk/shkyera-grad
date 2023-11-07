#pragma once

#include <random>

namespace shkyera::utils {

std::random_device rand_dev;
std::mt19937 generator(rand_dev());

template <typename T> std::enable_if_t<!std::is_integral_v<T>, T> sample(T from, T to) {
    std::uniform_real_distribution<T> distribution(from, to);
    return distribution(generator);
}

template <typename T> std::enable_if_t<!std::is_integral_v<T>, std::vector<T>> sample(T from, T to, size_t size) {
    std::uniform_real_distribution<T> distribution(from, to);

    std::vector<T> sampled(size);
    for (size_t i = 0; i < size; i++) {
        sampled[i] = distribution(generator);
    }

    return sampled;
}

template <typename T> std::enable_if_t<std::is_integral_v<T>, T> sample(T from, T to) {
    std::uniform_int_distribution<T> distribution(from, to);
    return distribution(generator);
}

template <typename T> std::enable_if_t<std::is_integral_v<T>, std::vector<T>> sample(T from, T to, size_t size) {
    std::uniform_int_distribution<T> distribution(from, to);

    std::vector<T> sampled(size);
    for (size_t i = 0; i < size; i++) {
        sampled[i] = distribution(generator);
    }

    return sampled;
}

} // namespace shkyera::utils

/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

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

template <typename T>
std::enable_if_t<std::is_integral_v<T>, std::vector<T>> sample(T from, T to, size_t size, bool withReplacement = true) {
    std::uniform_int_distribution<T> distribution(from, to);

    std::vector<T> sampled(size);
    for (size_t i = 0; i < size; i++) {
        T candidate = distribution(generator);
        while (!withReplacement && std::find(sampled.begin(), sampled.end(), candidate) != sampled.end())
            candidate = distribution(generator);
        sampled[i] = candidate;
    }

    return sampled;
}

template <typename T> void shuffle(std::vector<T> &vec) { std::shuffle(vec.begin(), vec.end(), rand_dev); }

template <typename Clock = std::chrono::high_resolution_clock> auto startTimer() { return Clock::now(); }

template <typename Clock = std::chrono::high_resolution_clock>
double stopTimer(const typename Clock::time_point &start) {
    auto end = Clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return static_cast<double>(duration.count()) / 1e6;
}

} // namespace shkyera::utils

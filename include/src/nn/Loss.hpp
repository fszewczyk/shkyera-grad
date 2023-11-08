/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../core/Value.hpp"

namespace shkyera::Loss {

template <typename T> using Function = std::function<ValuePtr<T>(Vector<T> a, Vector<T> b)>;

using Function32 = Function<Type::float32>;
using Function64 = Function<Type::float64>;

template <typename T>
Function<T> MSE = [](Vector<T> a, Vector<T> b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors need to be of the same size to compute the MSE loss. Sizes are " +
                                    std::to_string(a.size()) + " and " + std::to_string(b.size()) + ".");
    }

    ValuePtr<T> loss = Value<T>::create(0);
    for (size_t i = 0; i < a.size(); ++i) {
        loss = loss + ((a[i] - b[i])->pow(Value<T>::create(2)));
    }

    if (a.size() > 0)
        loss = loss / Value<T>::create(a.size());

    loss->backward();

    return loss;
};

template <typename T>
Function<T> MAE = [](Vector<T> a, Vector<T> b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors need to be of the same size to compute the MAE loss. Sizes are " +
                                    std::to_string(a.size()) + " and " + std::to_string(b.size()) + ".");
    }

    ValuePtr<T> loss = Value<T>::create(0);
    for (size_t i = 0; i < a.size(); ++i) {
        ValuePtr<T> difference = a[i] > b[i] ? a[i] - b[i] : b[i] - a[i];
        loss = loss + difference;
    }

    if (a.size() > 0)
        loss = loss / Value<T>::create(a.size());

    loss->backward();

    return loss;
};

} // namespace shkyera::Loss

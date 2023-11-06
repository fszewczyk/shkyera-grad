#pragma once

#include "Value.hpp"

namespace shkyera::Loss {

template <typename T> using Function = std::function<ValuePtr<T>(Vector<T> a, Vector<T> b)>;

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

    loss->backward();

    return loss;
};

} // namespace shkyera::Loss

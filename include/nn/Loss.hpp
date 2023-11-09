/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../core/Value.hpp"
#include "../core/Vector.hpp"

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

    return loss;
};

template <typename T>
Function<T> CrossEntropy = [](Vector<T> a, Vector<T> b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument(
            "Vectors need to be of the same size to compute the Cross Entropy loss. Sizes are " +
            std::to_string(a.size()) + " and " + std::to_string(b.size()) + ".");
    }

    auto aSum = a.sum();
    auto bSum = b.sum();

    if (aSum->getValue() < 0.99 || aSum->getValue() > 1.01 || aSum->getValue() < 0.99 || aSum->getValue() > 1.01) {
        throw std::invalid_argument("To compute Cross Entropy Loss, both elements of each vector need to sum to 1(+/- "
                                    "0.01). Currently, they sum to:" +
                                    std::to_string(aSum->getValue()) + " and " + std::to_string(bSum->getValue()) +
                                    ".");
    }

    auto loss = Value<T>::create(0);
    for (size_t i = 0; i < a.size(); ++i) {
        loss = loss - (b[i] * (a[i]->log()));
    }

    return loss;
};

template <typename T>
ValuePtr<T> compute(Function<T> lossFunction, const Vector<T> prediction, const Vector<T> target) {
    auto loss = lossFunction(prediction, target);
    loss->backward();
    return loss;
}

template <typename T> ValuePtr<T> compute(Function<T> lossFunction, const Batch<T> prediction, const Batch<T> target) {
    ValuePtr<T> loss = Value<T>::create(0);
    for (size_t i = 0; i < prediction.size(); ++i) {
        loss = loss + lossFunction(prediction[i], target[i]);
    }
    loss = loss / Value<T>::create(prediction.size());

    loss->backward();

    return loss;
}

} // namespace shkyera::Loss

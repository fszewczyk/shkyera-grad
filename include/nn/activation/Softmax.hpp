/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "Activation.hpp"

namespace shkyera {

template <typename T> class Softmax;
using Softmax32 = Softmax<Type::float32>;
using Softmax64 = Softmax<Type::float64>;

template <typename T> class Softmax : public Activation<T> {
  public:
    static std::shared_ptr<Softmax<T>> create();

    virtual Vector<T> operator()(const Vector<T> &x) const override;
};

template <typename T> std::shared_ptr<Softmax<T>> Softmax<T>::create() {
    return std::shared_ptr<Softmax<T>>(new Softmax<T>());
}

template <typename T> Vector<T> Softmax<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> out;
    out.reserve(x.size());

    auto maxValue = Value<T>::create(x[0]->getValue());
    for (size_t i = 1; i < x.size(); ++i)
        if (x[i] > maxValue)
            maxValue = x[i];

    auto sumExponentiated = Value<T>::create(0);
    for (size_t i = 0; i < x.size(); ++i) {
        auto exponentiated = (x[i] - maxValue)->exp();
        out.emplace_back(exponentiated);
        sumExponentiated = sumExponentiated + exponentiated;
    }

    auto vectorizedOut = Vector<T>(out) / sumExponentiated;

    return vectorizedOut;
}

} // namespace shkyera

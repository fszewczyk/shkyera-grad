/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "Activation.hpp"

namespace shkyera {

template <typename T> class Sigmoid;
using Sigmoid32 = Sigmoid<Type::float32>;
using Sigmoid64 = Sigmoid<Type::float64>;

template <typename T> class Sigmoid : public Activation<T> {
  public:
    static std::shared_ptr<Sigmoid<T>> create();

    virtual Vector<T> operator()(const Vector<T> &x) const override;
};

template <typename T> std::shared_ptr<Sigmoid<T>> Sigmoid<T>::create() {
    return std::shared_ptr<Sigmoid<T>>(new Sigmoid<T>());
}

template <typename T> Vector<T> Sigmoid<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> out;
    out.reserve(x.size());

    for (auto &entry : x) {
        out.emplace_back(entry->sigmoid());
    }

    return Vector<T>(out);
}

} // namespace shkyera

/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "Activation.hpp"

namespace shkyera {

template <typename T> class ReLU;
using ReLU32 = ReLU<Type::float32>;
using ReLU64 = ReLU<Type::float64>;

template <typename T> class ReLU : public Activation<T> {
  public:
    static std::shared_ptr<ReLU<T>> create();

    virtual Vector<T> operator()(const Vector<T> &x) const override;
};

template <typename T> std::shared_ptr<ReLU<T>> ReLU<T>::create() { return std::shared_ptr<ReLU<T>>(new ReLU<T>()); }

template <typename T> Vector<T> ReLU<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> out;
    out.reserve(x.size());

    for (auto &entry : x)
        out.emplace_back(entry->relu());

    return Vector<T>(out);
}

} // namespace shkyera

/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "Activation.hpp"

namespace shkyera {

template <typename T> class Exp;
using Exp32 = Exp<Type::float32>;
using Exp64 = Exp<Type::float64>;

template <typename T> class Exp : public Activation<T> {
  public:
    static std::shared_ptr<Exp<T>> create();

    virtual Vector<T> operator()(const Vector<T> &x) const override;
};

template <typename T> std::shared_ptr<Exp<T>> Exp<T>::create() { return std::shared_ptr<Exp<T>>(new Exp<T>()); }

template <typename T> Vector<T> Exp<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> out;
    out.reserve(x.size());

    for (auto &entry : x)
        out.emplace_back(entry->exp());

    return Vector<T>(out);
}

} // namespace shkyera

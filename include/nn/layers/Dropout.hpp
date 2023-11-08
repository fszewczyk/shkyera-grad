/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../../core/Utils.hpp"
#include "Linear.hpp"

namespace shkyera {

template <typename T> class Dropout;
template <typename T> using DropoutPtr = std::shared_ptr<Dropout<T>>;

using Dropout32 = Dropout<Type::float32>;
using Dropout64 = Dropout<Type::float64>;

template <typename T> class Dropout : public Linear<T> {
  private:
    double _dropout;

    Dropout(size_t input, size_t size, double dropout);

  public:
    static DropoutPtr<T> create(size_t input, size_t size, double dropout);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
};

template <typename T> Dropout<T>::Dropout(size_t input, size_t size, double dropout) : Linear<T>(input, size) {
    if (_dropout < 0 || _dropout >= 1) {
        throw std::invalid_argument("Droput rate must be in the range [0,1). You set it to " + std::to_string(dropout) +
                                    ".");
    }
    _dropout = dropout;
}

template <typename T> DropoutPtr<T> Dropout<T>::create(size_t input, size_t size, double dropout) {
    return std::shared_ptr<Dropout<T>>(new Dropout(input, size, dropout));
}

template <typename T> Vector<T> Dropout<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> alteredInput;
    alteredInput.reserve(x.size());
    for (const ValuePtr<T> &val : x)
        alteredInput.push_back(val);

    std::vector<size_t> indicesToRemove = utils::sample<size_t>(0, x.size() - 1, _dropout * x.size(), false);
    for (size_t idxToRemove : indicesToRemove)
        alteredInput[idxToRemove] = Value<T>::create(0);

    auto transformedInput = Vector<T>(alteredInput) * static_cast<T>(1.0 / (1 - _dropout));

    return Linear<T>::operator()(transformedInput);
}

} // namespace shkyera

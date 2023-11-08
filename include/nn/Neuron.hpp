/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../core/Type.hpp"
#include "../core/Utils.hpp"
#include "../core/Value.hpp"
#include "../core/Vector.hpp"
#include "Module.hpp"

namespace shkyera {

template <typename T> class Neuron;
using Neuron32 = Neuron<Type::float32>;
using Neuron64 = Neuron<Type::float64>;

template <typename T> class Neuron {
  private:
    ValuePtr<T> _bias;
    Vector<T> _weights;

  public:
    Neuron(size_t input);

    Vector<T> operator()(const Vector<T> &x) const;
    std::vector<ValuePtr<T>> parameters() const;
};

template <typename T> Neuron<T>::Neuron(size_t input) {
    auto weights = utils::sample<T>(-1, 1, input);

    _weights = Vector<T>::of(weights);
    _bias = Value<T>::create(utils::sample<T>(-1, 1));
}

template <typename T> Vector<T> Neuron<T>::operator()(const Vector<T> &x) const {
    return Vector<T>({_bias + _weights.dot(x)});
}

template <typename T> std::vector<ValuePtr<T>> Neuron<T>::parameters() const {
    std::vector<ValuePtr<T>> params;
    params.reserve(_weights.size() + 1);

    for (size_t i = 0; i < _weights.size(); ++i)
        params.push_back(_weights[i]);

    params.push_back(_bias);

    return params;
}

} // namespace shkyera

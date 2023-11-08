/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../../core/Type.hpp"
#include "../Module.hpp"
#include "../Neuron.hpp"

namespace shkyera {

template <typename T> class Linear;
template <typename T> using LinearPtr = std::shared_ptr<Linear<T>>;

using Linear32 = Linear<Type::float32>;
using Linear64 = Linear<Type::float64>;

template <typename T> class Linear : public Module<T> {
  protected:
    std::vector<Neuron<T>> _neurons;

    Linear(size_t input, size_t size);

  public:
    static LinearPtr<T> create(size_t input, size_t size);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
    virtual std::vector<ValuePtr<T>> parameters() const override;
};

template <typename T> Linear<T>::Linear(size_t input, size_t size) {
    _neurons.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _neurons.emplace_back(Neuron<T>(input));
    }
}

template <typename T> LinearPtr<T> Linear<T>::create(size_t input, size_t size) {
    return std::shared_ptr<Linear<T>>(new Linear<T>(input, size));
}

template <typename T> Vector<T> Linear<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> output(_neurons.size());

    for (size_t i = 0; i < _neurons.size(); i++) {
        output[i] = _neurons[i](x)[0];
    }

    return Vector<T>(output);
}

template <typename T> std::vector<ValuePtr<T>> Linear<T>::parameters() const {
    std::vector<ValuePtr<T>> params;
    for (const Neuron<T> &n : _neurons) {
        std::vector<ValuePtr<T>> neuronParams = n.parameters();
        params.insert(params.end(), neuronParams.begin(), neuronParams.end());
    }

    return params;
}

} // namespace shkyera

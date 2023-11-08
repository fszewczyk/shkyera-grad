/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../core/Type.hpp"
#include "Activation.hpp"
#include "Module.hpp"
#include "Neuron.hpp"

namespace shkyera {

template <typename T> class Layer;
template <typename T> using LayerPtr = std::shared_ptr<Layer<T>>;

using Layer32 = Layer<Type::float32>;
using Layer64 = Layer<Type::float32>;

template <typename T> class Layer : public Module<T> {
  private:
    std::vector<Neuron<T>> _neurons;

    Layer(size_t input, size_t size, Activation::Function<T> activation = Activation::relu<T>);

  public:
    static LayerPtr<T> create(size_t input, size_t size, Activation::Function<T> activation = Activation::relu<T>);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
    virtual std::vector<ValuePtr<T>> parameters() const override;
};

template <typename T> Layer<T>::Layer(size_t input, size_t size, Activation::Function<T> activation) {
    _neurons.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _neurons.emplace_back(Neuron<T>(input, activation));
    }
}

template <typename T> LayerPtr<T> Layer<T>::create(size_t input, size_t size, Activation::Function<T> activation) {
    return std::shared_ptr<Layer<T>>(new Layer<T>(input, size, activation));
}

template <typename T> Vector<T> Layer<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> output(_neurons.size());

    for (size_t i = 0; i < _neurons.size(); i++) {
        output[i] = _neurons[i](x)[0];
    }

    return Vector<T>(output);
}

template <typename T> std::vector<ValuePtr<T>> Layer<T>::parameters() const {
    std::vector<ValuePtr<T>> params;
    for (const Neuron<T> &n : _neurons) {
        std::vector<ValuePtr<T>> neuronParams = n.parameters();
        params.insert(params.end(), neuronParams.begin(), neuronParams.end());
    }

    return params;
}

} // namespace shkyera

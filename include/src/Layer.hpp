#pragma once

#include "Activation.hpp"
#include "Neuron.hpp"

namespace shkyera {

template <typename T> class Layer {
  private:
    std::vector<Neuron<T>> _neurons;

  public:
    Layer(size_t size, size_t input, Activation::Function<T> activation = Activation::relu<T>);

    Vector<T> operator()(const Vector<T> &x) const;
};

template <typename T> Layer<T>::Layer(size_t size, size_t input, Activation::Function<T> activation) {
    _neurons.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _neurons.emplace_back(Neuron<T>(input, activation));
    }
}

template <typename T> Vector<T> Layer<T>::operator()(const Vector<T> &x) const {
    std::vector<ValuePtr<T>> output(_neurons.size());
    for (size_t i = 0; i < _neurons.size(); i++) {
        output[i] = _neurons[i](x);
    }
    return Vector<T>(output);
}

} // namespace shkyera

#pragma once

#include "Utils.hpp"
#include "Value.hpp"
#include "Vector.hpp"

namespace shkyera {

template <typename T> class Neuron {
  private:
    ValuePtr<T> _bias;
    Vector<T> _weights;
    std::function<ValuePtr<T>(ValuePtr<T>)> _activation = [](ValuePtr<T> a) { return a; };

  public:
    Neuron(size_t input);
    Neuron(size_t input, std::function<ValuePtr<T>(ValuePtr<T>)> activation);

    ValuePtr<T> operator()(const Vector<T> &x) const;
};

template <typename T> Neuron<T>::Neuron(size_t input) {
    _bias = Value<T>::create(utils::sample<T>(-1, 1));
    _weights = utils::sample<T>(-1, 1, input);
}

template <typename T>
Neuron<T>::Neuron(size_t input, std::function<ValuePtr<T>(ValuePtr<T>)> activation) : Neuron<T>(input) {
    _activation = activation;
}

template <typename T> ValuePtr<T> Neuron<T>::operator()(const Vector<T> &x) const {
    return _activation(_bias + _weights.dot(x));
}

} // namespace shkyera

#pragma once

#include "Activation.hpp"
#include "Module.hpp"
#include "Neuron.hpp"

namespace shkyera {

template <typename T> class Layer : public Module<T> {
  private:
    std::vector<Neuron<T>> _neurons;

  public:
    Layer(size_t input, size_t size, Activation::Function<T> activation = Activation::relu<T>);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
    virtual std::vector<ValuePtr<T>> parameters() const override;
};

template <typename T> Layer<T>::Layer(size_t input, size_t size, Activation::Function<T> activation) {
    _neurons.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        _neurons.emplace_back(Neuron<T>(input, activation));
    }
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

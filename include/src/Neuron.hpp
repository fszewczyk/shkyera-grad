#pragma once

#include "Module.hpp"
#include "Utils.hpp"
#include "Value.hpp"
#include "Vector.hpp"

namespace shkyera {

template <typename T> class Neuron : public Module<T> {
  private:
    ValuePtr<T> _bias;
    Vector<T> _weights;
    std::function<ValuePtr<T>(ValuePtr<T>)> _activation = [](ValuePtr<T> a) { return a; };

  public:
    Neuron(size_t input);
    Neuron(size_t input, std::function<ValuePtr<T>(ValuePtr<T>)> activation);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
    virtual std::vector<ValuePtr<T>> parameters() const override;
};

template <typename T> Neuron<T>::Neuron(size_t input) {
    auto weights = utils::sample<T>(-1, 1, input);

    _weights = Vector<T>::of(weights);
    _bias = Value<T>::create(utils::sample<T>(-1, 1));
}

template <typename T>
Neuron<T>::Neuron(size_t input, std::function<ValuePtr<T>(ValuePtr<T>)> activation) : Neuron<T>(input) {
    _activation = activation;
}

template <typename T> Vector<T> Neuron<T>::operator()(const Vector<T> &x) const {
    return Vector<T>({_activation(_bias + _weights.dot(x))});
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

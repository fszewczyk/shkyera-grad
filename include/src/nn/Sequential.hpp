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

namespace shkyera {

template <typename T> class Sequential;
template <typename T> class SequentialBuilder;
template <typename T> using SequentialPtr = std::shared_ptr<Sequential<T>>;

using Sequential32 = Sequential<Type::float32>;
using Sequential64 = Sequential<Type::float64>;
using SequentialBuilder32 = SequentialBuilder<Type::float32>;
using SequentialBuilder64 = SequentialBuilder<Type::float64>;

template <typename T> class Sequential : public Module<T> {
  private:
    std::vector<ModulePtr<T>> _layers;

    Sequential(const std::vector<ModulePtr<T>> &layers);

  public:
    static SequentialPtr<T> create(const std::vector<ModulePtr<T>> &layers);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
    virtual std::vector<ValuePtr<T>> parameters() const override;
};

template <typename T> class SequentialBuilder {
  private:
    std::vector<ModulePtr<T>> _layers;

    SequentialBuilder() = default;

  public:
    static SequentialBuilder<T> begin();

    SequentialBuilder<T> add(ModulePtr<T> layer);
    SequentialPtr<T> build();
};

template <typename T> Sequential<T>::Sequential(const std::vector<ModulePtr<T>> &layers) : _layers(layers) {}

template <typename T> SequentialPtr<T> Sequential<T>::create(const std::vector<ModulePtr<T>> &layers) {
    return std::shared_ptr<Sequential<T>>(new Sequential<T>(layers));
}

template <typename T> Vector<T> Sequential<T>::operator()(const Vector<T> &x) const {
    Vector<T> out = (*_layers[0])(x);

    std::for_each(_layers.begin() + 1, _layers.end(), [&out](ModulePtr<T> layer) { out = layer->forward(out); });

    return out;
}

template <typename T> std::vector<ValuePtr<T>> Sequential<T>::parameters() const {
    std::vector<ValuePtr<T>> params;

    for (const ModulePtr<T> &l : _layers) {
        std::vector<ValuePtr<T>> layerParams = l->parameters();
        params.insert(params.end(), layerParams.begin(), layerParams.end());
    }

    return params;
}

template <typename T> SequentialBuilder<T> SequentialBuilder<T>::begin() { return SequentialBuilder<T>(); }
template <typename T> SequentialBuilder<T> SequentialBuilder<T>::add(ModulePtr<T> layer) {
    _layers.push_back(layer);
    return *this;
}
template <typename T> SequentialPtr<T> SequentialBuilder<T>::build() { return Sequential<T>::create(_layers); }

} // namespace shkyera

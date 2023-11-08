/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <unordered_map>
#include <vector>

#include "../../core/Type.hpp"
#include "../../core/Value.hpp"
#include "../Module.hpp"
#include "Optimizer.hpp"

namespace shkyera {

template <typename T> class Adam;
using Adam32 = Adam<Type::float32>;
using Adam64 = Adam<Type::float32>;

template <typename T> class Adam : public Optimizer<T> {
  private:
    T _b1;
    T _b2;
    T _eps;
    size_t _timestep;

    std::unordered_map<Value<T> *, T> _firstMoment;
    std::unordered_map<Value<T> *, T> _secondMoment;

    T getFirstMoment(const ValuePtr<T> &v);
    T getSecondMoment(const ValuePtr<T> &v);

  public:
    Adam(std::vector<ValuePtr<T>> params, T learningRate, T b1 = 0.9, T b2 = 0.999, T eps = 1e-8);

    void step() override;
};

template <typename T>
Adam<T>::Adam(std::vector<ValuePtr<T>> params, T learningRate, T b1, T b2, T eps) : Optimizer<T>(params, learningRate) {
    _timestep = 0;
    _b1 = b1;
    _b2 = b2;
    _eps = eps;
}

template <typename T> void Adam<T>::step() {
    _timestep++;

    for (const ValuePtr<T> &param : this->_parameters) {
        T gradient = param->getGradient();

        T firstMoment = _b1 * getFirstMoment(param) + (1 - _b1) * gradient;
        T secondMoment = _b2 * getSecondMoment(param) + (1 - _b2) * gradient * gradient;

        _firstMoment.insert({param.get(), firstMoment});
        _secondMoment.insert({param.get(), secondMoment});

        T firstMomentHat = firstMoment / (1 - pow(_b1, _timestep));
        T secondMomentHat = secondMoment / (1 - pow(_b2, _timestep));

        param->_data -= (this->_learningRate * firstMomentHat) / (sqrt(secondMomentHat) + _eps);
    }
}

template <typename T> T Adam<T>::getFirstMoment(const ValuePtr<T> &v) {
    auto moment = _firstMoment.find(v.get());
    if (moment == _firstMoment.end()) {
        _firstMoment.insert({v.get(), 0});
        return 0;
    }
    return moment->second;
}

template <typename T> T Adam<T>::getSecondMoment(const ValuePtr<T> &v) {
    auto moment = _secondMoment.find(v.get());
    if (moment == _secondMoment.end()) {
        _secondMoment.insert({v.get(), 0});
        return 0;
    }
    return moment->second;
}

} // namespace shkyera

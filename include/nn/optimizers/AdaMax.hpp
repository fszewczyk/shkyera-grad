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

template <typename T> class AdaMax;
using AdaMax32 = AdaMax<Type::float32>;
using AdaMax64 = AdaMax<Type::float32>;

template <typename T> class AdaMax : public Optimizer<T> {
  private:
    size_t _timestep;
    T _b1;
    T _b2;
    T _eps;

    std::vector<T> _moments;
    std::vector<T> _infinityNorms;

  public:
    AdaMax(std::vector<ValuePtr<T>> params, T learningRate, T b1 = 0.9, T b2 = 0.999, T eps = 1e-8);

    void step() override;
};

template <typename T>
AdaMax<T>::AdaMax(std::vector<ValuePtr<T>> params, T learningRate, T b1, T b2, T eps)
    : Optimizer<T>(params, learningRate) {
    _b1 = b1;
    _b2 = b2;
    _eps = eps;

    _timestep = 0;
    _moments.resize(params.size(), 0);
    _infinityNorms.resize(params.size(), 0);
}

template <typename T> void AdaMax<T>::step() {
    ++_timestep;

    for (size_t i = 0; i < this->_parameters.size(); ++i) {
        const ValuePtr<T> &param = this->_parameters[i];

        T gradient = param->getGradient();
        T moment = _b1 * _moments[i] + (1 - _b1) * gradient;
        T infinityNorm = std::max(_b2 * _infinityNorms[i], std::abs(gradient) + _eps);

        param->_data -= (this->_learningRate / (1 - std::pow(_b1, _timestep))) * (moment / infinityNorm);

        _infinityNorms[i] = infinityNorm;
        _moments[i] = moment;
    }
}

} // namespace shkyera

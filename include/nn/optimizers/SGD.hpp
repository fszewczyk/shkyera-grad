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

template <typename T> class SGD;
using SGD32 = SGD<Type::float32>;
using SGD64 = SGD<Type::float32>;

template <typename T> class SGD : public Optimizer<T> {
  private:
    T _momentum;
    std::vector<T> _moments;

  public:
    SGD(std::vector<ValuePtr<T>> params, T learningRate, T momentum = 0.9);

    void step() override;
};

template <typename T>
SGD<T>::SGD(std::vector<ValuePtr<T>> params, T learningRate, T momentum) : Optimizer<T>(params, learningRate) {
    _momentum = momentum;
    _moments.resize(params.size(), 0);
}

template <typename T> void SGD<T>::step() {
    static bool initialized = false;

    for (size_t i = 0; i < this->_parameters.size(); ++i) {
        const ValuePtr<T> &param = this->_parameters[i];

        T gradient = param->getGradient();
        T moment = initialized ? _momentum * _moments[i] + (1 - _momentum) * gradient : gradient;
        _moments[i] = moment;

        param->_data -= this->_learningRate * moment;
    }
}

} // namespace shkyera

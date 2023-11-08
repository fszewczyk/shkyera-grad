/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <vector>

#include "../core/Type.hpp"
#include "../core/Value.hpp"
#include "Module.hpp"

namespace shkyera {

using Optimizer32 = Optimizer<Type::float32>;
using Optimizer64 = Optimizer<Type::float32>;

template <typename T> class Optimizer {
  private:
    std::vector<ValuePtr<T>> _parameters;
    T _learningRate;

  public:
    Optimizer(std::vector<ValuePtr<T>> params, T learningRate);

    void reset();
    void step();
};

template <typename T>
Optimizer<T>::Optimizer(std::vector<ValuePtr<T>> params, T learningRate) : _learningRate(learningRate) {
    _parameters = params;
}

template <typename T> void Optimizer<T>::reset() {
    std::for_each(_parameters.begin(), _parameters.end(), [](ValuePtr<T> val) { val->_gradient = 0; });
}

template <typename T> void Optimizer<T>::step() {
    std::for_each(_parameters.begin(), _parameters.end(),
                  [this](ValuePtr<T> val) { val->_data -= _learningRate * val->_gradient; });
}

} // namespace shkyera

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

    void resetGradient();
    void stepGradient();
};

template <typename T>
Optimizer<T>::Optimizer(std::vector<ValuePtr<T>> params, T learningRate) : _learningRate(learningRate) {
    _parameters = params;
}

template <typename T> void Optimizer<T>::resetGradient() {
    std::for_each(_parameters.begin(), _parameters.end(), [](ValuePtr<T> val) { val->_gradient = 0; });
}

template <typename T> void Optimizer<T>::stepGradient() {
    std::for_each(_parameters.begin(), _parameters.end(),
                  [this](ValuePtr<T> val) { val->_data -= _learningRate * val->_gradient; });
}

} // namespace shkyera

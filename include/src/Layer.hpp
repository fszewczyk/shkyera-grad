#pragma once

#include <memory>

#include "Activation.hpp"
#include "math/Matrix.hpp"
#include "math/Vector.hpp"

namespace st {

class Layer {
  public:
    Layer(size_t inputSize, size_t neurons, ACTIVATION activation = RELU);

    Vector<double> feedForward(Vector<double> input) const;

  private:
    Vector<double> activate(Vector<double> v) const;

    std::unique_ptr<Activation> m_activation;
    Matrix<double> m_weights;
    Vector<double> m_bias;
};

Layer::Layer(size_t inputSize, size_t neurons, ACTIVATION activation) {
    m_bias = Vector<double>::random(neurons);
    m_weights = Matrix<double>::random(neurons, inputSize);

    switch (activation) {
    case RELU:
        m_activation = std::make_unique<ReLU>();
        break;
    case SIGMOID:
        m_activation = std::make_unique<Sigmoid>();
        break;
    case TANH:
    default:
        m_activation = std::make_unique<TanH>();
        break;
    }
}

Vector<double> Layer::feedForward(Vector<double> input) const { return activate(m_weights.dot(input) + m_bias); }

Vector<double> Layer::activate(Vector<double> v) const {
    for (size_t i = 0; i < v.getSize(); ++i) {
        v(i) = m_activation->activate(v(i));
    }

    return v;
}

} // namespace st
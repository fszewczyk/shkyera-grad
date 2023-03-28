#pragma once

#include <math.h>

#include "math/Vector.hpp"

namespace st {

enum ACTIVATION { RELU, SIGMOID, TANH };

class Activation {
  public:
    virtual double activate(double x) const = 0;
    virtual double derivative(double x) const = 0;
};

class ReLU : public Activation {
    virtual double activate(double x) const override;
    virtual double derivative(double x) const override;
};

class Sigmoid : public Activation {
    virtual double activate(double x) const override;
    virtual double derivative(double x) const override;
};

class TanH : public Activation {
    virtual double activate(double x) const override;
    virtual double derivative(double x) const override;
};

double ReLU::activate(double x) const { return std::max(0.0, x); }
double ReLU::derivative(double x) const { return x > 0 ? 1 : 0; }

double Sigmoid::activate(double x) const { return 1 / (1 + exp(-x)); }
double Sigmoid::derivative(double x) const { return activate(x) * (1 - activate(x)); }

double TanH::activate(double x) const { return tanh(x); }
double TanH::derivative(double x) const { return 1 - pow(tanh(x), 2); }

} // namespace st
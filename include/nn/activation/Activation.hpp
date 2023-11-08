/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../../core/Type.hpp"
#include "../Module.hpp"

namespace shkyera {

template <typename T> class Activation;

template <typename T> class Activation : public Module<T> {
  protected:
    Activation() = default;

  public:
    virtual Vector<T> operator()(const Vector<T> &x) const override { return x; }
    virtual std::vector<ValuePtr<T>> parameters() const override { return {}; }
};

} // namespace shkyera

/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "../core/Vector.hpp"

namespace shkyera {

template <typename T> class Module;
template <typename T> using ModulePtr = std::shared_ptr<Module<T>>;

template <typename T> class Module {
  protected:
    Module() = default;

  public:
    Vector<T> forward(const Vector<T> &x) const { return (*this)(x); }
    virtual Vector<T> operator()(const Vector<T> &x) const { return x; }
    virtual std::vector<ValuePtr<T>> parameters() const { return {}; }
};

} // namespace shkyera

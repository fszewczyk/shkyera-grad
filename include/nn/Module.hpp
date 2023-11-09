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
    template <typename U> U forward(const U &x) const { return (*this)(x); }

    virtual Vector<T> operator()(const Vector<T> &x) const { return x; }
    std::vector<Vector<T>> operator()(const std::vector<Vector<T>> &x) const {
        std::vector<Vector<T>> out(x.size());
        for (size_t i = 0; i < x.size(); ++i) {
            out[i] = this->operator()(x[i]);
        }
        return out;
    }

    virtual std::vector<ValuePtr<T>> parameters() const { return {}; }
};

} // namespace shkyera

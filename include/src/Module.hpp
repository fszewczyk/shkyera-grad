#pragma once

#include "Vector.hpp"

namespace shkyera {

template <typename T> class Module {
  public:
    virtual Vector<T> operator()(const Vector<T> &x) const = 0;
    virtual std::vector<ValuePtr<T>> parameters() const = 0;
};

} // namespace shkyera

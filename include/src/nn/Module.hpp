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

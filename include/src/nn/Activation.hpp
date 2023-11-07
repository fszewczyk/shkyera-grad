#pragma once

#include "../core/Type.hpp"
#include "../core/Value.hpp"

namespace shkyera::Activation {

template <typename T> using Function = std::function<ValuePtr<T>(ValuePtr<T>)>;

template <typename T> Function<T> tanh = [](ValuePtr<T> a) { return a->tanh(); };
template <typename T> Function<T> relu = [](ValuePtr<T> a) { return a->relu(); };
template <typename T> Function<T> exp = [](ValuePtr<T> a) { return a->exp(); };
template <typename T> Function<T> sigmoid = [](ValuePtr<T> a) { return a->sigmoid(); };
template <typename T> Function<T> linear = [](ValuePtr<T> a) { return a; };

} // namespace shkyera::Activation

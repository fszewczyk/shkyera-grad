/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <exception>

#include "Type.hpp"
#include "Value.hpp"

namespace shkyera {

template <typename T> class Vector;

using Vec32 = Vector<Type::float32>;
using Vec64 = Vector<Type::float64>;
template <typename T> using Batch = std::vector<Vector<T>>;

template <typename T> class Vector {
  private:
    std::vector<ValuePtr<T>> _values;

  public:
    Vector() = default;
    Vector(std::vector<ValuePtr<T>> values);

    static Vector<T> of(const std::vector<T> &values);
    template <typename... Args> static Vector<T> of(const Args &...args);

    ValuePtr<T> dot(const Vector<T> &other) const;
    ValuePtr<T> sum() const;
    size_t size() const;

    template <typename U> friend std::ostream &operator<<(std::ostream &os, const Vector<U> &vector);

    template <typename U> friend Vector<U> operator/(Vector<U> x, U val);
    template <typename U> friend Vector<U> operator*(Vector<U> x, U val);
    template <typename U> friend Vector<U> operator/(Vector<U> x, ValuePtr<U> val);
    template <typename U> friend Vector<U> operator*(Vector<U> x, ValuePtr<U> val);
    Vector<T> &operator/=(T val);
    Vector<T> &operator*=(T val);
    Vector<T> &operator/=(ValuePtr<T> val);
    Vector<T> &operator*=(ValuePtr<T> val);

    ValuePtr<T> operator[](size_t index) const;

    class ConstIterator {
      private:
        size_t _index;
        const Vector<T> &_vector;

      public:
        ConstIterator(size_t index, const Vector<T> &vector);
        const ValuePtr<T> operator*();
        ConstIterator &operator++();
        bool operator!=(const ConstIterator &other);
    };

    ConstIterator begin() const;
    ConstIterator end() const;
};

template <typename T> Vector<T>::Vector(std::vector<ValuePtr<T>> values) { _values = values; }

template <typename T> Vector<T> Vector<T>::of(const std::vector<T> &values) {
    std::vector<ValuePtr<T>> valuePtrs;
    valuePtrs.reserve(values.size());

    std::for_each(values.begin(), values.end(),
                  [&valuePtrs](const T &v) { valuePtrs.emplace_back(Value<T>::create(v)); });

    return valuePtrs;
}

template <typename T> template <typename... Args> Vector<T> Vector<T>::of(const Args &...args) {
    std::vector<ValuePtr<T>> valuePtrs;
    valuePtrs.reserve(sizeof...(args));

    (valuePtrs.emplace_back(Value<T>::create(args)), ...);

    return Vector<T>(valuePtrs);
}

template <typename T> size_t Vector<T>::size() const { return _values.size(); }

template <typename T> ValuePtr<T> Vector<T>::dot(const Vector<T> &other) const {
    if (other.size() != size()) {
        throw std::invalid_argument("Vectors need to be of the same size to compute the dot product. Sizes are " +
                                    std::to_string(size()) + " and " + std::to_string(other.size()) + ".");
    }

    ValuePtr<T> result = Value<T>::create(0);
    for (size_t i = 0; i < size(); ++i)
        result = result + (_values[i] * other[i]);

    return result;
}

template <typename T> ValuePtr<T> Vector<T>::sum() const {
    auto sum = Value<T>::create(0);
    for (const auto &entry : _values)
        sum = sum + entry;
    return sum;
}

template <typename T> Vector<T> operator/(Vector<T> x, T val) {
    x *= Value<T>::create(val);
    return x;
}

template <typename T> Vector<T> operator*(Vector<T> x, T val) {
    x *= Value<T>::create(val);
    return x;
}

template <typename T> Vector<T> operator/(Vector<T> x, ValuePtr<T> val) {
    auto out = x;
    for (size_t i = 0; i < out._values.size(); ++i)
        out._values[i] = out._values[i] / val;
    return out;
}

template <typename T> Vector<T> operator*(Vector<T> x, ValuePtr<T> val) {
    auto out = x;
    for (size_t i = 0; i < out._values.size(); ++i)
        out._values[i] = out._values[i] * val;
    return out;
}

template <typename T> Vector<T> &Vector<T>::operator/=(T val) {
    auto divisor = Value<T>::create(val);
    for (size_t i = 0; i < _values.size(); ++i)
        _values[i] = _values[i] / divisor;
    return *this;
}

template <typename T> Vector<T> &Vector<T>::operator*=(T val) {
    auto divisor = Value<T>::create(val);
    for (size_t i = 0; i < _values.size(); ++i)
        _values[i] = _values[i] * divisor;
    return *this;
}

template <typename T> Vector<T> &Vector<T>::operator/=(ValuePtr<T> val) {
    for (size_t i = 0; i < _values.size(); ++i)
        _values[i] = _values[i] / val;
    return *this;
}

template <typename T> Vector<T> &Vector<T>::operator*=(ValuePtr<T> val) {
    for (size_t i = 0; i < _values.size(); ++i)
        _values[i] = _values[i] * val;
    return *this;
}

template <typename T> ValuePtr<T> Vector<T>::operator[](size_t index) const { return _values[index]; }

template <typename T> std::ostream &operator<<(std::ostream &os, const Vector<T> &vector) {
    os << "Vector(size=" << vector.size() << ", data={";

    for (auto val : vector)
        os << val << ' ';

    os << "})";
    return os;
}

template <typename T> typename Vector<T>::ConstIterator Vector<T>::begin() const { return ConstIterator(0, *this); }
template <typename T> typename Vector<T>::ConstIterator Vector<T>::end() const { return ConstIterator(size(), *this); }

template <typename T>
Vector<T>::ConstIterator::ConstIterator(size_t index, const Vector<T> &vector) : _index(index), _vector(vector) {}

template <typename T> const ValuePtr<T> Vector<T>::ConstIterator::operator*() {
    if (_index < _vector.size()) {
        return _vector[_index];
    }
    throw std::out_of_range("Vector iterator out of range. Tried to access index " + std::to_string(_index) +
                            " in a Vector of size " + std::to_string(_vector.size()) + ".");
}

template <typename T> typename Vector<T>::ConstIterator &Vector<T>::ConstIterator::operator++() {
    ++_index;
    return *this;
}

template <typename T> bool Vector<T>::ConstIterator::operator!=(const ConstIterator &other) {
    return _index != other._index;
}

} // namespace shkyera

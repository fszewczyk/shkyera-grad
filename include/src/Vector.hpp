#pragma once

#include <exception>

#include "Value.hpp"

namespace shkyera {

template <typename T> class Vector {
  private:
    std::vector<ValuePtr<T>> _values;

  public:
    Vector() = default;
    Vector(std::vector<ValuePtr<T>> values);
    static Vector<T> of(const std::vector<T> &values);

    ValuePtr<T> dot(const Vector<T> &other) const;
    ValuePtr<T> operator[](size_t index) const;

    size_t size() const;

    template <typename U> friend std::ostream &operator<<(std::ostream &os, const Vector<U> &vector);
};

template <typename T> Vector<T>::Vector(std::vector<ValuePtr<T>> values) { _values = values; }

template <typename T> Vector<T> Vector<T>::of(const std::vector<T> &values) {
    std::vector<ValuePtr<T>> valuePtrs;
    valuePtrs.reserve(values.size());

    std::for_each(values.begin(), values.end(),
                  [&valuePtrs](const T &v) { valuePtrs.emplace_back(Value<T>::create(v)); });

    return valuePtrs;
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

template <typename T> ValuePtr<T> Vector<T>::operator[](size_t index) const { return _values[index]; }

template <typename T> std::ostream &operator<<(std::ostream &os, const Vector<T> &vector) {
    os << "Vector(size=" << vector.size() << ", data={";

    for (const ValuePtr<T> val : vector._values)
        os << val << ' ';

    os << "})";
    return os;
}

} // namespace shkyera

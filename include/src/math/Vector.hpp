#pragma once

#include <math.h>
#include <stdlib.h>
#include <vector>

#include "Utils.hpp"

namespace st {

template <typename T> class Vector {
  public:
    Vector();
    Vector(std::vector<T> data);
    Vector(size_t size);
    Vector(size_t size, T value);

    static Vector<T> zeros(size_t size);
    static Vector<T> ones(size_t size);
    static Vector<T> random(size_t size, T low = -1, T high = 1);

    T norm() const;
    T max() const;

    void clear();
    void fill(T value);
    void append(T value);
    void resize(size_t size);

    void softmax();

    size_t getSize() const;
    std::vector<T> getValues() const;

    template <typename U> T dot(Vector<U> &v) const;

    T &operator()(size_t i);
    T operator[](size_t i) const;

    template <typename U> Vector<T> operator+(const U &v) const;
    template <typename U> Vector<T> operator-(const U &v) const;
    template <typename U> Vector<T> operator*(const U &v) const;
    template <typename U> Vector<T> operator/(const U &v) const;

    template <typename U> Vector<T> operator+(const Vector<U> &v) const;
    template <typename U> Vector<T> operator-(const Vector<U> &v) const;
    template <typename U> Vector<T> operator*(const Vector<U> &v) const;
    template <typename U> Vector<T> operator/(const Vector<U> &v) const;

    template <typename U> void operator+=(const U &v);
    template <typename U> void operator-=(const U &v);
    template <typename U> void operator*=(const U &v);
    template <typename U> void operator/=(const U &v);

    template <typename U> void operator+=(const Vector<U> &v);
    template <typename U> void operator-=(const Vector<U> &v);
    template <typename U> void operator*=(const Vector<U> &v);
    template <typename U> void operator/=(const Vector<U> &v);

  private:
    size_t m_size;
    std::vector<T> m_data;
};

template <typename T> Vector<T>::Vector() : m_size(0) {}

template <typename T> Vector<T>::Vector(std::vector<T> data) {
    m_size = data.size();
    m_data = data;
}

template <typename T> Vector<T>::Vector(size_t size) {
    m_size = size;
    m_data.resize(size);
}

template <typename T> Vector<T>::Vector(size_t size, T value) {
    m_size = size;
    m_data.resize(size);
    fill(value);
}

template <typename T> Vector<T> Vector<T>::zeros(size_t size) { return Vector<T>(size, 0); }
template <typename T> Vector<T> Vector<T>::ones(size_t size) { return Vector<T>(size, 1); }
template <typename T> Vector<T> Vector<T>::random(size_t size, T low, T high) {
    Vector<T> vec(size);

    for (size_t i = 0; i < size; ++i) {
        vec(i) = st::utils::randomValue<T>(low, high);
    }

    return vec;
}

template <typename T> T Vector<T>::norm() const {
    T sum = 0;
    for (T val : m_data) {
        sum += val * val;
    }

    return sqrt(sum);
}

template <typename T> T Vector<T>::max() const {
    T pMax = std::numeric_limits<T>::lowest();
    for (auto v : m_data) {
        pMax = std::max(pMax, v);
    }

    return pMax;
}

template <typename T> void Vector<T>::clear() {
    m_data.clear();
    m_size = 0;
}
template <typename T> void Vector<T>::fill(T value) { std::fill(m_data.begin(), m_data.end(), value); }
template <typename T> void Vector<T>::append(T value) {
    m_data.push_back(value);
    m_size++;
}

template <typename T> void Vector<T>::resize(size_t size) {
    m_size = size;
    m_data.resize(size);
}

template <typename T> void Vector<T>::softmax() {
    double sum = 0;

    for (const auto &v : m_data) {
        sum += exp(v);
    }

    for (size_t i = 0; i < getSize(); ++i) {
        m_data[i] = exp(m_data[i]) / sum;
    }
}

template <typename T> size_t Vector<T>::getSize() const { return m_size; }
template <typename T> std::vector<T> Vector<T>::getValues() const { return m_data; }

template <typename T> template <typename U> T Vector<T>::dot(Vector<U> &v) const {
    T result = 0;
    for (size_t i = 0; i < m_size; ++i) {
        result += m_data[i] * v[i];
    }

    return result;
}

template <typename T> T &Vector<T>::operator()(size_t i) { return m_data[i]; }
template <typename T> T Vector<T>::operator[](size_t i) const { return m_data[i]; }

template <typename T> template <typename U> Vector<T> Vector<T>::operator+(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] + v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator-(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] - v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator*(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] * v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator/(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] / v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator+(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] + v[i];
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator-(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] - v[i];
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator*(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] * v[i];
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator/(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result(i) = m_data[i] / v[i];
    }

    return result;
}

template <typename T> template <typename U> void Vector<T>::operator+=(const U &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] += v;
    }
}

template <typename T> template <typename U> void Vector<T>::operator-=(const U &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] -= v;
    }
}

template <typename T> template <typename U> void Vector<T>::operator*=(const U &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] *= v;
    }
}

template <typename T> template <typename U> void Vector<T>::operator/=(const U &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] /= v;
    }
}

template <typename T> template <typename U> void Vector<T>::operator+=(const Vector<U> &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] += v[i];
    }
}

template <typename T> template <typename U> void Vector<T>::operator-=(const Vector<U> &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] -= v[i];
    }
}

template <typename T> template <typename U> void Vector<T>::operator*=(const Vector<U> &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] *= v[i];
    }
}

template <typename T> template <typename U> void Vector<T>::operator/=(const Vector<U> &v) {
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i] /= v[i];
    }
}

template <typename T> inline std::ostream &operator<<(std::ostream &out, const Vector<T> &v) {
    out << "{";
    for (size_t i = 0; i < v.getSize(); ++i) {
        out << v[i];
        if (i + 1 != v.getSize())
            out << ",";
    }
    out << "}";

    return out;
}

} // namespace st

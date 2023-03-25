#include <stdlib.h>
#include <vector>

#pragma once

namespace st {

template <typename T> class Vector {
  public:
    Vector() = default;
    Vector(size_t size);
    Vector(size_t size, T value);

    T &operator[](size_t i);

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

    size_t getSize() const;

  private:
    void fill(T value);

    size_t m_size;
    std::vector<T> m_data;
};

template <typename T> Vector<T>::Vector(size_t size) {
    m_size = size;
    m_data.resize(size);
}

template <typename T> Vector<T>::Vector(size_t size, T value) {
    m_size = size;
    m_data.resize(size);
    fill(value);
}

template <typename T> T &Vector<T>::operator[](size_t i) { return m_data[i]; }

template <typename T> template <typename U> Vector<T> Vector<T>::operator+(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] + v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator-(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] - v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator*(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] * v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator/(const U &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] / v;
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator+(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] + v[i];
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator-(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] - v[i];
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator*(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] * v[i];
    }

    return result;
}

template <typename T> template <typename U> Vector<T> Vector<T>::operator/(const Vector<U> &v) const {
    Vector<T> result(m_size);
    for (size_t i = 0; i < m_size; ++i) {
        result[i] = m_data[i] / v[i];
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

template <typename T> size_t Vector<T>::getSize() const { return m_size; }

template <typename T> void Vector<T>::fill(T value) { std::fill(m_data.begin(), m_data.end(), value); }

} // namespace st
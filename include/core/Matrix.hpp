#pragma once

#include <math.h>
#include <stdlib.h>
#include <vector>

#include "Utils.hpp"
#include "Vector.hpp"

namespace st {

template <typename T> class Matrix {
  public:
    Matrix() = default;
    Matrix(std::vector<std::vector<T>> data);
    Matrix(size_t rows, size_t columns);
    Matrix(size_t rows, size_t columns, T value);

    static Matrix<T> zeros(size_t rows, size_t columns);
    static Matrix<T> ones(size_t rows, size_t columns);
    static Matrix<T> random(size_t rows, size_t columns, T low = -1, T high = 1);

    void clear();
    void fill(T value);

    std::pair<size_t, size_t> getSize() const;
    std::vector<std::vector<T>> getValues() const;

    Vector<T> getRow(size_t i) const;
    Vector<T> getColumn(size_t i) const;

    template <typename U> Vector<T> dot(Vector<U> &v) const;

    T &operator()(size_t row, size_t column);
    T at(size_t row, size_t column) const;

    template <typename U> Matrix<T> operator+(const U &v) const;
    template <typename U> Matrix<T> operator-(const U &v) const;
    template <typename U> Matrix<T> operator*(const U &v) const;
    template <typename U> Matrix<T> operator/(const U &v) const;

    template <typename U> Matrix<T> operator+(const Matrix<U> &v) const;
    template <typename U> Matrix<T> operator-(const Matrix<U> &v) const;
    template <typename U> Matrix<T> operator*(const Matrix<U> &v) const;

    template <typename U> void operator+=(const U &v);
    template <typename U> void operator-=(const U &v);
    template <typename U> void operator*=(const U &v);
    template <typename U> void operator/=(const U &v);

    template <typename U> void operator+=(const Matrix<U> &v);
    template <typename U> void operator-=(const Matrix<U> &v);
    template <typename U> void operator*=(const Matrix<U> &v);

  private:
    size_t m_rows;
    size_t m_columns;

    std::vector<std::vector<T>> m_data;
};

template <typename T> Matrix<T>::Matrix(std::vector<std::vector<T>> data) {
    m_rows = data.size();
    m_columns = m_rows > 0 ? data[0].size() : 0;

    m_data = data;
}

template <typename T> Matrix<T>::Matrix(size_t rows, size_t columns) {
    m_rows = rows;
    m_columns = columns;

    m_data.resize(rows);
    for (size_t row = 0; row < m_rows; ++row) {
        m_data[row].resize(m_columns);
    }
}

template <typename T> Matrix<T>::Matrix(size_t rows, size_t columns, T value) {
    m_rows = rows;
    m_columns = columns;

    m_data.resize(rows);
    for (size_t row = 0; row < m_rows; ++row) {
        m_data[row].resize(m_columns);
    }

    fill(value);
}

template <typename T> Matrix<T> Matrix<T>::zeros(size_t rows, size_t columns) { return Matrix<T>(rows, columns, 0); }
template <typename T> Matrix<T> Matrix<T>::ones(size_t rows, size_t columns) { return Matrix<T>(rows, columns, 1); }
template <typename T> Matrix<T> Matrix<T>::random(size_t rows, size_t columns, T low, T high) {
    Matrix<T> mat(rows, columns);

    for (size_t row = 0; row < rows; ++row) {
        for (size_t col = 0; col < columns; ++col) {
            mat(row, col) = st::utils::randomValue<T>(low, high);
        }
    }

    return mat;
}

template <typename T> void Matrix<T>::clear() {
    m_data.clear();
    m_rows = 0;
    m_columns = 0;
}

template <typename T> void Matrix<T>::fill(T value) {
    for (size_t row = 0; row < m_rows; ++row) {
        std::fill(m_data[row].begin(), m_data[row].end(), value);
    }
}

template <typename T> std::pair<size_t, size_t> Matrix<T>::getSize() const { return {m_rows, m_columns}; }
template <typename T> std::vector<std::vector<T>> Matrix<T>::getValues() const { return m_data; }

template <typename T> Vector<T> Matrix<T>::getRow(size_t i) const {
    if (i > m_rows)
        throw std::invalid_argument("Index of a row is higher than matrix's size.");
    return Vector<T>(m_data[i]);
}
template <typename T> Vector<T> Matrix<T>::getColumn(size_t i) const {
    Vector<T> col(m_rows);
    for (size_t row = 0; row < m_rows; ++row) {
        col(row) = at(row, i);
    }
    return col;
}

template <typename T> template <typename U> Vector<T> Matrix<T>::dot(Vector<U> &v) const {
    Vector<T> result(m_rows);

    for (size_t row = 0; row < m_rows; ++row) {
        Vector<T> rowVector(m_data[row]);
        result(row) = v.dot(rowVector);
    }

    return result;
}

template <typename T> T &Matrix<T>::operator()(size_t row, size_t column) { return m_data[row][column]; }
template <typename T> T Matrix<T>::at(size_t row, size_t column) const { return m_data[row][column]; }

template <typename T> inline std::ostream &operator<<(std::ostream &out, const Matrix<T> &m) {
    out << "{";
    for (size_t row = 0; row < m.getSize().first; ++row) {
        out << Vector<T>(m.getValues()[row]);
        if (row + 1 != m.getSize().first)
            out << ",\n";
    }
    out << "}";

    return out;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator+(const U &v) const {
    Matrix<T> result(m_rows, m_columns);
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            result(row, col) = m_data[row][col] + v;
        }
    }

    return result;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator-(const U &v) const {
    Matrix<T> result(m_rows, m_columns);
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            result(row, col) = m_data[row][col] - v;
        }
    }

    return result;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator*(const U &v) const {
    Matrix<T> result(m_rows, m_columns);

    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            result(row, col) = m_data[row][col] * v;
        }
    }

    return result;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator/(const U &v) const {
    Matrix<T> result(m_rows, m_columns);
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            result(row, col) = m_data[row][col] / v;
        }
    }

    return result;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator+(const Matrix<U> &v) const {
    if (getSize() != v.getSize()) {
        throw std::invalid_argument("Adding matrices of different sizes is not allowed");
    }

    Matrix<T> result(m_rows, m_columns);
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            result(row, col) = m_data[row][col] + v(row, col);
        }
    }

    return result;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator-(const Matrix<U> &v) const {
    if (getSize() != v.getSize()) {
        throw std::invalid_argument("Subtracting matrices of different sizes is not allowed");
    }

    Matrix<T> result(m_rows, m_columns);
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            result(row, col) = m_data[row][col] - v(row, col);
        }
    }

    return result;
}

template <typename T> template <typename U> Matrix<T> Matrix<T>::operator*(const Matrix<U> &m) const {
    auto [mRows, mColumns] = m.getSize();
    if (m_columns != mRows) {
        throw std::invalid_argument("Matrices to multiply do not have matching sizes");
    }

    Matrix<T> result(m_rows, mColumns);

    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < mColumns; ++col) {
            auto rowVector = getRow(row);
            auto colVector = m.getColumn(col);

            result(row, col) = rowVector.dot(colVector);
        }
    }

    return result;
}

template <typename T> template <typename U> void Matrix<T>::operator+=(const U &v) {
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            m_data[row][col] += v;
        }
    }
}

template <typename T> template <typename U> void Matrix<T>::operator-=(const U &v) {
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            m_data[row][col] -= v;
        }
    }
}

template <typename T> template <typename U> void Matrix<T>::operator*=(const U &v) {
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            m_data[row][col] *= v;
        }
    }
}

template <typename T> template <typename U> void Matrix<T>::operator/=(const U &v) {
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            m_data[row][col] /= v;
        }
    }
}

template <typename T> template <typename U> void Matrix<T>::operator+=(const Matrix<U> &m) {
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            m_data[row][col] += m.at(row, col);
        }
    }
}

template <typename T> template <typename U> void Matrix<T>::operator-=(const Matrix<U> &m) {
    for (size_t row = 0; row < m_rows; ++row) {
        for (size_t col = 0; col < m_columns; ++col) {
            m_data[row][col] -= m.at(row, col);
        }
    }
}

template <typename T> template <typename U> void Matrix<T>::operator*=(const Matrix<U> &m) {
    auto result = *this * m;

    auto [resultRows, resultCols] = result.getSize();
    m_rows = resultRows;
    m_columns = resultCols;

    m_data = result.getValues();
}

} // namespace st

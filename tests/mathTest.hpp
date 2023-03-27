#include <iostream>

#include "../include/ShkyeraTensor.hpp"

namespace st::tests {
void matrixTimesMatrix();
void matrixTimesVector();
void vectorTimesVector();

void runMath();
} // namespace st::tests

void st::tests::matrixTimesMatrix() {
    auto m1 = st::Matrix<int>::ones(3, 3);
    auto m2 = st::Matrix<int>::zeros(3, 3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m2(i, j) = i + j;
        }
    }

    m1 *= m2;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int expected = 3 * (j + 1);
            if (m1(i, j) != expected)
                throw std::runtime_error("st::tests::matrixTimesMatrix() yields incorrect result");
        }
    }
}

void st::tests::matrixTimesVector() {
    auto m = st::Matrix<int>::ones(3, 3);
    st::Vector<int> v(3, 2);

    v = m.dot(v);

    for (int i = 0; i < 3; i++) {
        if (v[i] != 6)
            throw std::runtime_error("st::tests::matrixTimesVector() yields incorrect result");
    }
}

void st::tests::vectorTimesVector() {
    st::Vector<int> v1(3, 2);
    st::Vector<int> v2(3, 3);
    int res = v1.dot(v2);

    if (res != 18)
        throw std::runtime_error("st::tests::vectorTimesVector() yields incorrect result");
}

void st::tests::runMath() {
    st::tests::vectorTimesVector();
    st::tests::matrixTimesVector();
    st::tests::matrixTimesMatrix();
}

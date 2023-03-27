#include <iostream>

#include "ShkyeraTensor.hpp"

int main() {
    auto vec = st::Vector<int>::random(3, -5, 5);
    auto mat = st::Matrix<int>::random(2, 4, -5, 5);
    auto mat1 = st::Matrix<int>::random(4, 3, -5, 5);

    mat *= mat1;

    if (true)
        throw std::invalid_argument("Testing Actions :0");

    return 0;
}

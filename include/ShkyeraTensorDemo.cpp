#include <iostream>

#include "ShkyeraTensor.hpp"

int main() {
    st::Vector<float> vec(3, 4.5);

    std::cerr << vec[0] << '\n';

    return 0;
}
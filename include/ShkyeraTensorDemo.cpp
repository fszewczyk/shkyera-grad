#include <iostream>

#include "ShkyeraTensor.hpp"

int main() {
    st::Vector<float> vec({2, 4, 5});
    st::Vector<float> vec1({20, 40, 50});

    vec += vec1;

    std::cerr << vec[0] << '\n';
    std::cerr << vec.norm() << '\n';

    return 0;
}
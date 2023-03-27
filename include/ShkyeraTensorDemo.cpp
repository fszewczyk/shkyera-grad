#include <iostream>

#include "ShkyeraTensor.hpp"

int main() {
    auto vec = st::Vector<int>::random(10);

    std::cerr << vec << '\n';
    std::cerr << vec.norm() << '\n';

    return 0;
}

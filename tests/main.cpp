#include "../include/ShkyeraTensor.hpp"

int main() {
    using namespace shkyera;

    auto x = Vector<double>({2, 3});
    auto layer = Layer<double>(5, 2, Activation::tanh<double>);

    auto a = layer(x);

    std::cerr << a << '\n';
}

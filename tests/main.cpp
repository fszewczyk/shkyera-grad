#include "../include/ShkyeraTensor.hpp"

int main() {
    using namespace shkyera;

    auto n = Neuron<double>(5, Activation::tanh<double>);
    auto x = Vector<double>({1, 2, 3, 4, 5});

    auto a = n(x);

    std::cerr << a << '\n';
}

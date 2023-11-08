#include "../include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;

    auto a = Value<Type::float32>::create(1);
    auto b = Value<Type::float32>::create(2);
    auto c = (a + b)->tanh();
    auto d = (a - b + c)->sigmoid();
    a = (b * c)->exp();
    b = (c / d)->relu();
    auto e = b->pow(d);

    // Calculating the gradients
    e->backward();

    for (auto val : {a, b, c, d, e}) {
        std::cout << "Value: " << val << " Gradient de/dval: " << val->getGradient() << std::endl;
    }
}

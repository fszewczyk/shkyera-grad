#include "../include/ShkyeraTensor.hpp"

int main() {
    using namespace shkyera;

    auto a = Value<double>::create(1);
    auto b = a * a;
    auto c = b / Value<double>::create(7);
    auto d = c->tanh();
    auto e = d->pow(Value<double>::create(2));
    auto f = e->exp();

    std::cerr << f << '\n';
    f->backward();

    for (auto v : {a, b, c, d, e, f})
        std::cerr << v->getGradient() << '\n';
}

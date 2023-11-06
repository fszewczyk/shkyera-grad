#include "../include/ShkyeraTensor.hpp"

int main() {
    using namespace shkyera;

    std::vector<Vector<double>> xs = {Vector<double>::of({0, 0}), Vector<double>::of({1, 0}),
                                      Vector<double>::of({0, 1}), Vector<double>::of({1, 1})};
    std::vector<Vector<double>> ys = {Vector<double>::of({0}), Vector<double>::of({1}), Vector<double>::of({1}),
                                      Vector<double>::of({0})};

    auto mlp = MLP<double>(2, {3, 1}, {Activation::tanh<double>, Activation::tanh<double>});
    auto lossFunction = Loss::MSE<double>;

    for (size_t epoch = 0; epoch < 10; epoch++) {
        for (size_t sample = 0; sample < xs.size(); ++sample) {
            auto pred = mlp(xs[sample]);
            auto loss = lossFunction(pred, ys[sample]);

            std::cerr << loss << '\n';
        }
    }
}

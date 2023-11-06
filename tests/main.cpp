#include "../include/ShkyeraTensor.hpp"

int main() {
    using namespace shkyera;

    using T = Type::float32;

    std::vector<Vector<T>> xs = {Vector<T>::of({0, 0}), Vector<T>::of({1, 0}), Vector<T>::of({0, 1}),
                                 Vector<T>::of({1, 1})};
    std::vector<Vector<T>> ys = {Vector<T>::of({0}), Vector<T>::of({1}), Vector<T>::of({1}), Vector<T>::of({0})};

    auto mlp = MLP<T>(2, {5, 5, 1}, {Activation::relu<T>, Activation::relu<T>, Activation::sigmoid<T>});
    auto optimizer = Optimizer<T>(mlp.parameters(), 0.1);
    auto lossFunction = Loss::MSE<T>;

    for (size_t epoch = 0; epoch < 1000; epoch++) {
        optimizer.resetGradient();

        for (size_t sample = 0; sample < xs.size(); ++sample) {

            auto pred = mlp(xs[sample]);
            auto loss = lossFunction(pred, ys[sample]);

            std::cerr << loss << '\n';
        }

        optimizer.stepGradient();
    }
}

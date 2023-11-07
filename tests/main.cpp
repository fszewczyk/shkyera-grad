#include "../include/ShkyeraTensor.hpp"

int main() {
    using namespace shkyera;

    std::vector<Vec32> xs = {Vec32::of({0, 0}), Vec32::of({1, 0}), Vec32::of({0, 1}), Vec32::of({1, 1})};
    std::vector<Vec32> ys = {Vec32::of({0}), Vec32::of({1}), Vec32::of({1}), Vec32::of({0})};

    // clang-format off
    auto mlp = SequentialBuilder<Type::float32>::begin()
                .add(Layer32::create(2, 15, Activation::relu<Type::float32>))
                .add(Layer32::create(15, 5, Activation::relu<Type::float32>))
                .add(Layer32::create(5, 1, Activation::sigmoid<Type::float32>))
                .build();
    // clang-format on

    auto optimizer = Optimizer<Type::float32>(mlp->parameters(), 0.1);
    auto lossFunction = Loss::MSE<Type::float32>;

    for (size_t epoch = 0; epoch < 1000; epoch++) {
        optimizer.resetGradient();
        for (size_t sample = 0; sample < xs.size(); ++sample) {

            auto pred = mlp->forward(xs[sample]);
            auto loss = lossFunction(pred, ys[sample]);

            std::cerr << loss << '\n';
        }
        optimizer.stepGradient();
    }
}

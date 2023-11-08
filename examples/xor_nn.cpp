#include "../include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;

    // clang-format off
    std::vector<Vec32> xs;
    std::vector<Vec32> ys;

    // ---------- INPUT ----------- | -------- OUTPUT --------- //
    xs.push_back(Vec32::of({0, 0})); ys.push_back(Vec32::of({0}));
    xs.push_back(Vec32::of({1, 0})); ys.push_back(Vec32::of({1}));
    xs.push_back(Vec32::of({0, 1})); ys.push_back(Vec32::of({1}));
    xs.push_back(Vec32::of({0, 0})); ys.push_back(Vec32::of({0}));

    auto mlp = SequentialBuilder<Type::float32>::begin()
                .add(Linear32::create(2, 15))
                .add(ReLU<Type::float32>::create())
                .add(Dropout32::create(15, 5, 0.2))
                .add(ReLU<Type::float32>::create())
                .add(Linear32::create(5, 1))
                .add(Sigmoid<Type::float32>::create())
                .build();
    // clang-format on

    Optimizer32 optimizer = Optimizer<Type::float32>(mlp->parameters(), 0.1);
    Loss::Function32 lossFunction = Loss::MSE<Type::float32>;

    // ------ TRAINING THE NETWORK ------- //
    for (size_t epoch = 0; epoch < 100; epoch++) {
        auto epochLoss = Val32::create(0);

        optimizer.reset();
        for (size_t sample = 0; sample < xs.size(); ++sample) {
            Vec32 pred = mlp->forward(xs[sample]);
            auto loss = lossFunction(pred, ys[sample]);

            epochLoss = epochLoss + loss;
        }
        optimizer.step();

        std::cout << "Epoch: " << epoch + 1 << " Loss: " << epochLoss->getValue() << std::endl;
    }

    // ------ VERIFYING THAT IT WORKS ------//
    for (size_t sample = 0; sample < xs.size(); ++sample) {
        Vec32 pred = mlp->forward(xs[sample]);
        std::cout << xs[sample] << " -> " << pred[0] << "\t| True: " << ys[sample][0] << std::endl;
    }
}

#include "../include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;

    // clang-format off
    std::vector<Vec32> xs;
    std::vector<Vec32> ys;

    // ---------- INPUT ----------- | -------- OUTPUT --------- //
    xs.push_back(Vec32::of(0, 0)); ys.push_back(Vec32::of(1, 0));
    xs.push_back(Vec32::of(1, 0)); ys.push_back(Vec32::of(0, 1));
    xs.push_back(Vec32::of(0, 1)); ys.push_back(Vec32::of(0, 1));
    xs.push_back(Vec32::of(1, 1)); ys.push_back(Vec32::of(1, 0));

    auto mlp = SequentialBuilder<Type::float32>::begin()
                .add(Linear32::create(2, 15))
                .add(ReLU32::create())
                .add(Linear32::create(15, 5))
                .add(Sigmoid32::create())
                .add(Linear32::create(5, 5))
                .add(Tanh32::create())
                .add(Linear32::create(5, 2))
                .add(Softmax32::create())
                .build();
    // clang-format on

    Adam32 optimizer = Adam<Type::float32>(mlp->parameters(), 0.1);
    Loss::Function32 lossFunction = Loss::CrossEntropy<Type::float32>;

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

        std::cout << "Epoch: " << epoch + 1 << " Loss: " << epochLoss->getValue() / xs.size() << std::endl;
    }

    // ------ VERIFYING THAT IT WORKS ------//
    for (size_t sample = 0; sample < xs.size(); ++sample) {
        Vec32 pred = mlp->forward(xs[sample]);
        std::cout << xs[sample] << " -> " << pred << "\t| True: " << ys[sample] << std::endl;
    }
}

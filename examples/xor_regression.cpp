#include "../include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;
    using T = Type::float32;

    // clang-format off
    std::vector<Vec32> xs;
    std::vector<Vec32> ys;

    // ---------- INPUT ----------- | -------- OUTPUT --------- //
    xs.push_back(Vec32::of(0, 0)); ys.push_back(Vec32::of(0));
    xs.push_back(Vec32::of(1, 0)); ys.push_back(Vec32::of(1));
    xs.push_back(Vec32::of(0, 1)); ys.push_back(Vec32::of(1));
    xs.push_back(Vec32::of(1, 1)); ys.push_back(Vec32::of(0));

    auto network = SequentialBuilder<Type::float32>::begin()
                    .add(Linear32::create(2, 15))
                    .add(ReLU32::create())
                    .add(Linear32::create(15, 5))
                    .add(ReLU32::create())
                    .add(Linear32::create(5, 1))
                    .add(Sigmoid32::create())
                    .build();
    // clang-format on

    auto optimizer = Adam32(network->parameters(), 0.05);
    auto lossFunction = Loss::MSE<T>;

    for (size_t epoch = 0; epoch < 100; epoch++) { // We train for 100 epochs
        auto epochLoss = Val32::create(0);

        optimizer.reset();                                      // Reset the gradients
        for (size_t sample = 0; sample < xs.size(); ++sample) { // We go through each sample
            Vec32 pred = network->forward(xs[sample]);          // We get some prediction
            auto loss = lossFunction(pred, ys[sample]);         // And calculate its error

            epochLoss = epochLoss + loss; // Store the loss for feedback
        }
        optimizer.step(); // Update the parameters

        auto averageLoss = epochLoss / Val32::create(xs.size());
        std::cout << "Epoch: " << epoch + 1 << " Loss: " << averageLoss->getValue() << std::endl;
    }

    for (size_t sample = 0; sample < xs.size(); ++sample) { // Go through each example
        Vec32 pred = network->forward(xs[sample]);          // Predict result
        std::cout << xs[sample] << " -> " << pred[0] << "\t| True: " << ys[sample][0] << std::endl;
    }
}

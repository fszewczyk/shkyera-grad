#include "../include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;
    using T = Type::float32;

    // clang-format off
    // This is our XOR dataset. It maps from Vec32 to Vec32
    Dataset<Vec32, Vec32> data;
    data.addSample(Vec32::of(0, 0), Vec32::of(0));
    data.addSample(Vec32::of(0, 1), Vec32::of(1));
    data.addSample(Vec32::of(1, 0), Vec32::of(1));
    data.addSample(Vec32::of(1, 1), Vec32::of(0));

    // The is the data loader, it will take care of batching
    size_t batchSize = 2;
    bool shuffle = true;
    DataLoader loader(data, batchSize, shuffle);

    auto network = SequentialBuilder<Type::float32>::begin()
                    .add(Linear32::create(2, 15))
                    .add(ReLU32::create())
                    .add(Linear32::create(15, 5))
                    .add(ReLU32::create())
                    .add(Linear32::create(5, 1))
                    .add(Sigmoid32::create())
                    .build();
    // clang-format on

    auto optimizer = Adam32(network->parameters(), 0.1);
    auto lossFunction = Loss::MSE<T>;

    for (size_t epoch = 0; epoch < 100; epoch++) { // We train for 100 epochs
        auto epochLoss = Val32::create(0);

        optimizer.reset();                                                // Reset the gradients
        for (const auto &[x, y] : loader) {                               // For each batch
            auto pred = network->forward(x);                              // We get some prediction
            epochLoss = epochLoss + Loss::compute(lossFunction, pred, y); // And calculate its error
        }
        optimizer.step(); // Update the parameters

        auto averageLoss = epochLoss / Val32::create(loader.getTotalBatches());
        std::cout << "Epoch: " << epoch + 1 << " Loss: " << averageLoss->getValue() << std::endl;
    }

    for (auto &[x, y] : data) {          // Go through each example
        auto pred = network->forward(x); // We get some prediction
        std::cout << x << " -> " << pred[0] << "\t| True: " << y[0] << std::endl;
    }
}

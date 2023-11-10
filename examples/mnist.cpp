#include <filesystem>
#include <iostream>

#include "../include/ShkyeraGrad.hpp"

namespace fs = std::filesystem;
using namespace shkyera;

Dataset<Vec32, Vec32> load(std::string directory) {
    Dataset<Vec32, Vec32> dataset;

    std::cerr << "Loading [" << std::flush;
    for (size_t digit = 0; digit < 10; ++digit) {
        std::cerr << "▮" << std::flush;
        int added = 0;
        for (const auto &entry : fs::directory_iterator(directory + std::to_string(digit))) {
            Image image(entry.path().string());
            auto target = Vec32::oneHotEncode(digit, 10);

            dataset.addSample(image.flatten<Type::float32>() / 255.0f, target);
        }
    }
    std::cerr << "]" << std::endl;

    return dataset;
}

int main() {
    Dataset<Vec32, Vec32> trainData = load("datasets/mnist/train/");
    std::cerr << "Loaded training data." << std::endl;

    DataLoader trainLoader(trainData, 16, true);

    // clang-format off
    auto mlp = SequentialBuilder32::begin()
                .add(Linear32::create(784, 100))
                .add(ReLU32::create())
                .add(Linear32::create(100, 50))
                .add(Sigmoid32::create())
                .add(Linear32::create(50, 10))
                .add(Softmax32::create())
                .build();
    // clang-format on

    auto optimizer = Adam32(mlp->parameters(), 0.01, 0.99);
    auto lossFunction = Loss::CrossEntropy<Type::float32>;

    for (size_t epoch = 0; epoch < 50; epoch++) {
        float epochLoss = 0;
        double epochAccuracy = 0;

        for (const auto [x, y] : trainLoader) {
            optimizer.reset();

            auto pred = mlp->forward(x);

            double accuracy = 0;
            for (size_t i = 0; i < pred.size(); ++i) {
                size_t predictedDigit = pred[i].argMax();
                size_t trueDigit = y[i].argMax();

                if (predictedDigit == trueDigit)
                    accuracy += 1;
            }

            accuracy /= pred.size();
            epochAccuracy += accuracy;

            auto loss = Loss::compute(lossFunction, pred, y);
            epochLoss = epochLoss + loss->getValue();

            optimizer.step();

            std::cerr << "Loss: " << loss->getValue() << " Accuracy: " << accuracy << std::endl;
        }
        std::cerr << "Epoch: " << epoch + 1 << " Loss: " << epochLoss / trainLoader.getTotalBatches()
                  << " Accuracy: " << epochAccuracy / trainLoader.getTotalBatches() << std::endl;
    }
}

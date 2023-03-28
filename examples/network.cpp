#include <iostream>

#include "../include/ShkyeraTensor.hpp"

int main() {
    /***************CSV***************/
    st::Dataset data("tests/preview.csv"); //< The first column is assumed to be the labels
    data.shuffle();                        //< Randomly shuffles the data
    auto [trainData, testData] =
        data.splitIntoTrainAndTest(0.6); //< Divides dataset into train and test set, with 0.6 proportion

    // Creating a neural network with 3 hidden layers with sizes 50,10,70
    // First layer has sigmoid activation function, second one uses tanh
    // Third one uses ReLU, because that's the default one
    st::Network nn(trainData, {50, 10, 70}, {st::SIGMOID, st::TANH});

    // Training the neural network with learning rate 0.01, batch size 7 for 10 epochs
    nn.train(0.01, 7, 10);
}

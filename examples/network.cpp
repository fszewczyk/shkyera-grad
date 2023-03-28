#include <iostream>

#include "../include/ShkyeraTensor.hpp"

int main() {
    /***************CSV***************/
    st::Dataset data("tests/preview.csv"); //< The first column is assumed to be the labels
    data.shuffle();                        //< Randomly shuffles the data
    auto [trainData, testData] =
        data.splitIntoTrainAndTest(0.6); //< Divides dataset into train and test set, with 0.6 proportion

    st::Network nn(trainData, {50, 10, 70}, {st::SIGMOID, st::TANH});
    nn.train(0.01, 7, 10);
}

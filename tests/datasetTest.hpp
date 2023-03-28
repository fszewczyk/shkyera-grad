#include <iostream>

#include "../include/ShkyeraTensor.hpp"

namespace st::tests {
void loadMNIST();

void runDataset();
} // namespace st::tests

void st::tests::loadMNIST() {
    st::Dataset data("tests/preview.csv"); //< The first column is assumed to be the labels
    data.shuffle();                        //< Randomly shuffles the data
    auto [trainData, testData] =
        data.splitIntoTrainAndTest(0.6); //< Divides dataset into train and test set, with 0.6 proportion

    auto headTestData = testData.head(); //< Gets first 5 rows of the Dataset
    for (auto row : headTestData) {
        if (row.getData().max() > 1)
            throw std::runtime_error(
                "st::tests::loadMNIST yields incorrect resutls. Normalization did not work as expected.");
    }
}

void st::tests::runDataset() { st::tests::loadMNIST(); }
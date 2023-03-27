#include <iostream>

#include "../include/ShkyeraTensor.hpp"

int main() {
    /***************CSV***************/
    st::Dataset<int, double> data("tests/preview.csv"); //< The first column is assumed to be the labels
    data.shuffle();                                     //< Randomly shuffles the data
    auto [trainData, testData] =
        data.splitIntoTrainAndTest(0.6); //< Divides dataset into train and test set, with 0.6 proportion

    auto headTestData = testData.head(); //< Gets first 5 rows of the Dataset
    for (auto row : headTestData) {
        std::cerr << row << '\n';
    }
}

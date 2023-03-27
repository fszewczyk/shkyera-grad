#include "../include/ShkyeraTensor.hpp"

#include "datasetTest.hpp"
#include "mathTest.hpp"

int main() {
    st::tests::runMath();
    st::tests::runDataset();

    std::cerr << "Success!" << std::endl;
}

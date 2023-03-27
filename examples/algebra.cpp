#include "../include/ShkyeraTensor.hpp"

int main() {
    /***************VECTOR***************/
    st::Vector<int> vInt(3);            // vector of 3 integers
    st::Vector<double> vDouble(3, 2.5); // vector of size 3, filled with 2.5 doubles
    auto randomFloatVec =
        st::Vector<float>::random(5, -3, 2);      // float vector of size 5, filled with random values between -3 and 2
    auto zerosIntVec = st::Vector<int>::zeros(4); // zero vector of size 4
    auto onesDoubleVec = st::Vector<double>::ones(5); // zero vector of size 4

    vDouble *= 2; // Multiplying vector by scalar
    vDouble -= 5; // Subtracting scalar from vector element-wise

    /***************MATRIX***************/
    st::Matrix<int> twosMat(4, 3, 2);                          // 4x3 integer matrix filled with 2
    auto randomMat = st::Matrix<double>::random(5, 4, -2, -1); // 5x4 matrix with random doubles between -2 and 1
    auto onesMat = st::Matrix<double>::ones(4, 2);             // 5x4 matrix with random ones

    auto dotProduct = randomMat.dot(onesDoubleVec);  // Dot product
    auto matrixMultiplication = randomMat * onesMat; // Multiplying matrices

    randomMat *= onesMat; // Multiplying matrices in-place

    randomMat += 3; // Adding a constants element-wise
    randomMat *= 2; // Multiplying by a constant element-wise

    return 0;
}

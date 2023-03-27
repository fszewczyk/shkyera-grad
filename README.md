<div align="center">
 
<h1>Shkyera Tensor</h1>

<div>

![LinuxBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/linux.yml/badge.svg) 
![MacOSBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/macos.yml/badge.svg) 
![MacOSBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/windows.yml/badge.svg) 
[![LICENSE](https://img.shields.io/badge/license-Beerware-yellow)](LICENSE) 

</div>

</div>

## Usage
See `/examples` for more detailed usage.

### Quick start
Simply import the library in your code
```cpp
#include "include/ShkyeraTensor.hpp"
```

### Dataset
```cpp
st::Dataset<int, double> data("tests/preview.csv"); //< The first column is assumed to be the labels
data.shuffle();                                     //< Randomly shuffles the data
std::pair<st::Dataset<int,double>, st::Dataset<int,double>> trainAndTest = data.splitIntoTrainAndTest(0.6); //< Divides dataset into train and test set, with 0.6 proportion
st::Dataset<int,double> headTestData = trainAndTest.second.head(); //< Gets first 5 rows of the Dataset
```

### Linear Algebra
#### Vector
```cpp
st::Vector<int> vInt(3); // vector of 3 integers
st::Vector<double> vDouble(3,2.5); // vector of size 3, filled with 2.5 doubles
auto randomFloatVec = st::Vector<float>::random(5, -3, 2); // float vector of size 5, filled with random values between -3 and 2
auto zerosIntVec = st::Vector<int>::zeros(4); // zero vector of size 4

vDouble *= 2; // Myltiplying vector by scalar
vDouble -= 5; // Subtracting scalar from vector element-wise
```

#### Matrix
```cpp
st::Matrix<int> twosMat(4, 3, 2);                          // 4x3 integer matrix filled with 2
auto randomMat = st::Matrix<double>::random(5, 4, -2, -1); // 5x4 matrix with random doubles between -2 and 1
auto onesMat = st::Matrix<double>::ones(4, 2);             // 5x4 matrix with random ones

auto dotProduct = randomMat.dot(st ::Vector<double>(5, 1)); // Dot product
auto matrixMultiplication = randomMat * onesMat;            // Multiplying matrices

randomMat *= onesMat; // Multiplying matrices in-place

randomMat += 3; // Adding a constants element-wise
randomMat *= 2; // Multiplying by a constant element-wise
```

## Roadmap
- [X] Basic Linear Algebra
- [X] Loading `.csv` datasets
- [ ] Basic Machine Learning
- [ ] Basic MLP
- [ ] Tensor
- [ ] Simple Reccurent Layers
- [ ] Echo State Networks
- [ ] LSTM Layer
- [ ] Transformers
- [ ] Convolutional Neural Networks

## Contributing
### Formatting
Make sure you use the appropriate formatting settings. Simple run this in the main directory:
```
clang-format -style='{ColumnLimit: 120, IndentWidth: 4}' -dump-config > .clang-format
```

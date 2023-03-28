<div align="center">
 
<h1>Shkyera Tensor</h1>

<div>

![LinuxBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/linux.yml/badge.svg) 
![MacOSBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/macos.yml/badge.svg) 
![MacOSBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/windows.yml/badge.svg) 
[![LICENSE](https://img.shields.io/badge/license-Beerware-yellow)](LICENSE) 

</div>
<i>First Deep Learning library <b>without</b> an implementation of a tensor</i>
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
// The first column is assumed to be the labels
// Labels are catgorical
st::Dataset<int, double> data("tests/preview.csv", CATEGORICAL); 

// Shuffles the data
data.shuffle();            

// Divides dataset into train and test set, with 0.6 proportion                         
std::pair<st::Dataset<int,double>, st::Dataset<int,double>> trainAndTest = data.splitIntoTrainAndTest(0.6); 

// Gets first 5 rows of the Dataset
st::Dataset<int,double> headTestData = trainAndTest.second.head(); 
```

### Network
```cpp
// Creating a neural network with 3 hidden layers
// First one has 50 neurons and uses sigmoid activation
// Second one has 30 neurons and uses sigmoid activation
// First one has 10 neurons and uses ReLU activation (default one)
st::Network nn(data, {50, 30, 10}, {st::SIGMOID, st::TANH});

// Train the network with 0.01 learning rate, batch size of 64 for 10 epochs
nn.train(0.01, 64, 10);
```

### Linear Algebra
#### Vector
```cpp
// Vector of 3 integers
st::Vector<int> vInt(3); 

// Vector of size 3, filled with 2.5 doubles
st::Vector<double> vDouble(3,2.5); 

// Float Vector of size 5, filled with random values between -3 and 2
auto randomFloatVec = st::Vector<float>::random(5, -3, 2); 

// Zero Vector of size 4
auto zerosIntVec = st::Vector<int>::zeros(4); 

// Sample algebra operations
st::Vector<float> resultDot = vDouble.dot(vInt);
resultDot *= 2; 
resultDot -= 5; 
```

#### Matrix
```cpp
// 4x3 integer matrix filled with 2
st::Matrix<int> twosMat(4, 3, 2);          

// 5x4 matrix with random doubles between -2 and 1
auto randomMat = st::Matrix<double>::random(5, 4, -2, -1); 

// 5x4 matrix with random ones
auto onesMat = st::Matrix<double>::ones(4, 2);             

// Sample algebra operations
auto dotProduct = randomMat.dot(st ::Vector<double>(5, 1)); 
auto matrixMultiplication = randomMat * onesMat;           
randomMat *= onesMat; 
randomMat += 3;
randomMat *= 2;
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

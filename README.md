<div align="center">
 
<h1>Shkyera Tensor</h1>

<div>

![LinuxBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/linux.yml/badge.svg) 
![MacOSBuild](https://github.com/fszewczyk/shkyera-tensor/actions/workflows/macos.yml/badge.svg) 
[![LICENSE](https://img.shields.io/badge/license-Beerware-yellow)](LICENSE) 

</div>

</div>

## Usage
See `/examples` for more detailed usage.

### Quick start
Simply import the library in your code
```cpp
#include <include/ShkyeraTensor.hpp>
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
auto randomMat = st::Matrix<double>::random(5,4,-2,-1); // 5x4 matrix with random doubles between -2 and 1
auto onesMat = st::Matrix<double>::ones(4,2); // 5x4 matrix with random ones

auto dotProduct = randomMat.dot(Vector<double>(5,1)) // Dot product 
auto matrixMultiplication = mat * mat2; // Multiplying matrices
mat *= mat2; // Multiplying matrices in-place

mat += 3; // Adding a constants element-wise
mat *= 2; // Multiplying by a constant element-wise
```

## Roadmap
- [X] Basic Linear Algebra
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

<div align="center">
 
<h1>Shkyera Grad</h1>

<i>
micrograd, but in C++ and better.
</i>
<p></p>

[![Documentation](https://github.com/fszewczyk/shkyera-grad/actions/workflows/docs.yml/badge.svg)](https://fszewczyk.github.io/shkyera-grad/index.html)
[![LinuxBuild](https://github.com/fszewczyk/shkyera-grad/actions/workflows/linux.yml/badge.svg)](https://github.com/fszewczyk/shkyera-grad/actions/workflows/linux.yml)
[![MacOSBuild](https://github.com/fszewczyk/shkyera-grad/actions/workflows/macos.yml/badge.svg)](https://github.com/fszewczyk/shkyera-grad/actions/workflows/macos.yml)
[![WindowsBuild](https://github.com/fszewczyk/shkyera-grad/actions/workflows/windows.yml/badge.svg)](https://github.com/fszewczyk/shkyera-grad/actions/workflows/windows.yml)
[![LICENSE](https://img.shields.io/badge/license-Beerware-yellow)](https://github.com/fszewczyk/shkyera-grad/blob/master/LICENSE)

</div>

This is a small header-only library of a scalar-valued autograd based on [Andrej Karpathy's micrograd](https://github.com/karpathy/micrograd). It provides a high-level, PyTorch-like API for creating and training simple neural networks.

It supports multiple optimizers, such as Adam or SGD, all the most common activation functions and basic types of neural layers. All of it wrapped in a simple, header-only library.

## Usage

Check out oour [Get Started Guide](https://fszewczyk.github.io/shkyera-grad/md_docs_tutorials_GetStarted.html) to learn the basics of _Shkyera Engine_.

## Showcase

Here's a small example showcasing a feed-forward network learning the XOR function. Check out the `examples/` folder for more examples.

```cpp
#include "shkyera-grad/include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;
    using T = Type::float32;

    // This is our XOR dataset. It maps from Vec32 to Vec32
    Dataset<Vec32, Vec32> data;
    data.addSample(Vec32::of(0, 0), Vec32::of(0));
    data.addSample(Vec32::of(0, 1), Vec32::of(1));
    data.addSample(Vec32::of(1, 0), Vec32::of(1));
    data.addSample(Vec32::of(1, 1), Vec32::of(0));

    // The is the data loader, it will take care of batching
    size_t batchSize = 2;
    bool shuffle = true;
    DataLoader loader(data, batchSize, shuffle);

    auto network = SequentialBuilder<Type::float32>::begin()
                    .add(Linear32::create(2, 15))
                    .add(ReLU32::create())
                    .add(Linear32::create(15, 5))
                    .add(ReLU32::create())
                    .add(Linear32::create(5, 1))
                    .add(Sigmoid32::create())
                    .build();

    auto optimizer = Adam32(network->parameters(), 0.1);
    auto lossFunction = Loss::MSE<T>;

    for (size_t epoch = 0; epoch < 100; epoch++) { // We train for 100 epochs
        auto epochLoss = Val32::create(0);

        optimizer.reset();                                                // Reset the gradients
        for (const auto &[x, y] : loader) {                               // For each batch
            auto pred = network->forward(x);                              // We get some prediction
            epochLoss = epochLoss + Loss::compute(lossFunction, pred, y); // And calculate its error
        }
        optimizer.step(); // Update the parameters

        auto averageLoss = epochLoss / Val32::create(loader.getTotalBatches());
        std::cout << "Epoch: " << epoch + 1 << " Loss: " << averageLoss->getValue() << std::endl;
    }

    for (auto &[x, y] : data) {          // Go through each example
        auto pred = network->forward(x); // We get some prediction
        std::cout << x << " -> " << pred[0] << "\t| True: " << y[0] << std::endl;
    }
}
```

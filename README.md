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

```{.cpp}
#include "shkyera-grad/include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;
    using T = Type::float32;

    std::vector<Vec32> xs;
    std::vector<Vec32> ys;

    // ---------- INPUT ----------- | -------- OUTPUT --------- //
    xs.push_back(Vec32::of({0, 0})); ys.push_back(Vec32::of({0}));
    xs.push_back(Vec32::of({1, 0})); ys.push_back(Vec32::of({1}));
    xs.push_back(Vec32::of({0, 1})); ys.push_back(Vec32::of({1}));
    xs.push_back(Vec32::of({1, 1})); ys.push_back(Vec32::of({0}));

    auto network = SequentialBuilder<Type::float32>::begin()
                    .add(Linear32::create(2, 15))
                    .add(ReLU32::create())
                    .add(Linear32::create(15, 5))
                    .add(ReLU32::create())
                    .add(Linear32::create(5, 1))
                    .add(Sigmoid32::create())
                    .build();


    auto optimizer = Adam32(network->parameters(), 0.05);
    auto lossFunction = Loss::MSE<T>;

    for (size_t epoch = 0; epoch < 100; epoch++) { // We train for 100 epochs
        auto epochLoss = Val32::create(0);

        optimizer.reset();                                      // Reset the gradients
        for (size_t sample = 0; sample < xs.size(); ++sample) { // We go through each sample
            Vec32 pred = network->forward(xs[sample]);          // We get some prediction
            auto loss = lossFunction(pred, ys[sample]);         // And calculate its error

            epochLoss = epochLoss + loss; // Store the loss for feedback
        }
        optimizer.step(); // Update the parameters

        auto averageLoss = epochLoss / Val32::create(xs.size());
        std::cout << "Epoch: " << epoch + 1 << " Loss: " << averageLoss->getValue() << std::endl;
    }

    for (size_t sample = 0; sample < xs.size(); ++sample) { // Go through each example
        Vec32 pred = network->forward(xs[sample]);          // Predict result
        std::cout << xs[sample] << " -> " << pred[0] << "\t| True: " << ys[sample][0] << std::endl;
    }
}
```

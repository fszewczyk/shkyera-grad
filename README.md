<div align="center">
 
<h1>Shkyera Grad</h1>
<i>
 micrograd, but in C++ and with more functionality.
</i>
<p></p>

</div>


This is a small header-only library of a scalar-valued autograd based on [Andrej Karpathy's micrograd](https://github.com/karpathy/micrograd). It provides a high-level, PyTorch-like API for creating and training simple neural networks.

## Usage

Make sure your compiler supports C++17. Shkyera Grad is a header-only library, so the only thing you need to do is to include it in your project.

```cpp
#include "include/ShkyeraGrad.hpp"
```

Check out the [examples](examples/README.md) for a quick start on Shkyera Grad. In the meantime, here's a neural network that learns the XOR function.

```cpp
#include "include/ShkyeraGrad.hpp"

int main() {
    using namespace shkyera;

    std::vector<Vec32> xs;
    std::vector<Vec32> ys;

    // ---------- INPUT ----------- | -------- OUTPUT --------- //
    xs.push_back(Vec32::of({0, 0})); ys.push_back(Vec32::of({0}));
    xs.push_back(Vec32::of({1, 0})); ys.push_back(Vec32::of({1}));
    xs.push_back(Vec32::of({0, 1})); ys.push_back(Vec32::of({1}));
    xs.push_back(Vec32::of({0, 0})); ys.push_back(Vec32::of({0}));

    auto mlp = SequentialBuilder<Type::float32>::begin()
                .add(Layer32::create(2, 15, Activation::relu<Type::float32>))
                .add(Layer32::create(15, 5, Activation::relu<Type::float32>))
                .add(Layer32::create(5, 1, Activation::sigmoid<Type::float32>))
                .build();

    Optimizer32 optimizer = Optimizer<Type::float32>(mlp->parameters(), 0.1);
    Loss::Function32 lossFunction = Loss::MSE<Type::float32>;

    // ------ TRAINING THE NETWORK ------- //
    for (size_t epoch = 0; epoch < 100; epoch++) {
        auto epochLoss = Val32::create(0);

        optimizer.reset();
        for (size_t sample = 0; sample < xs.size(); ++sample) {
            Vec32 pred = mlp->forward(xs[sample]);
            auto loss = lossFunction(pred, ys[sample]);

            epochLoss = epochLoss + loss;
        }
        optimizer.step();

        std::cout << "Epoch: " << epoch + 1 << " Loss: " << epochLoss->getValue() << std::endl;
    }
}
```

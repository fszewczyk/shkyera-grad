# Cheatsheet

This page contains all the info you need to develop your models using Shkyera Grad.

## Types

Almost all of the classes in _Shkyera Grad_ are implemented using templates. To simplify creation of these objects, we introduced a standard way to instantiate objects with floating-point template parameters, i.e.

```{.cpp}
Linear32 = Linear<float>
Optimizer32 = Optimizer<Type::float32>>
Loss::MSE64 = Loss::MSE<double>
Adam64 = Adam<Type::f64>

{Class}32 = {Class}<Type::float32> = {Class}<float>
{Class}64 = {Class}<Type::float64> = {Class}<double>
```

## Vectors

Here are all the available operations using `Vector`:

```{.cpp}
auto a = Vector<float>::of(1, 2, 3);
auto b = Vector<float>::of({2, 3, 4}); 

a.size() // 3
a.dot(b) // 1 * 2 + 2 * 3 + 3 * 4 = 20
a.sum()  // 6
a *= 3   // a = {3, 6, 9}
a /= 2   // a = {1.5, 3, 4.5}
a[1]     // 3
```

## Layers

Here's a full list of available layers:

```{.cpp}
auto linear = Linear32::create(inputSize, outputSize);
auto dropout = Dropout32::create(inputSize, outputSize, dropoutRate);
```

## Optimizers

These are all implemented optimizers:

```{.cpp}
auto simple = Optimizer32(network->parameters(), learningRate);
auto sgdWithMomentum = SGD32(network->parameters(), learningRate, momentum = 0.9);
auto adam = Adam32(network->parameters(), learningRate, beta1 = 0.9, beta2 = 0.999, epsilon = 1e-8);
```

## Loss functions

Optimization can be performed according to these predefined loss functions:

```{.cpp}
auto L1 = Loss::MAE32;
auto L2 = Loss::MSE32;
auto crossEntropy = Loss::CrossEntropy32;
```

## Generic Training Loop

Simply copy-pase this code to quickly train your network:

```{.cpp}
using T = Type::float32; // feel free to change it to float64

auto optimizer = Adam<T>(network->parameters(), 0.05);
auto lossFunction = Loss::MSE<T>;

for (size_t epoch = 0; epoch < 100; epoch++) {
    auto epochLoss = Value<T>::create(0);

    optimizer.reset();
    for (size_t sample = 0; sample < xs.size(); ++sample) {
        Vector<T> pred = network->forward(xs[sample]);
        auto loss = lossFunction(pred, ys[sample]);

        epochLoss = epochLoss + loss;
    }
    optimizer.step();

    auto averageLoss = epochLoss / Value<T>::create(xs.size());
    std::cout << "Epoch: " << epoch + 1 << " Loss: " << averageLoss->getValue() << std::endl;
}
```

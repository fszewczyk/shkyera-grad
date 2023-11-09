# Get Started

Hello! Let's get right into it. By the end of this guide, you will have created and trained your first neural in _Shkyera Grad_!

## Setup

This is easy, _Shkyera Grad_ is a header-only library, so simply clone the repositoryu into your project:

```
git clone https://github.com/fszewczyk/shkyera-grad.git
```

and import the main file of the library inside your own project.

```{.cpp}
#include "shkyera-grad/include/ShkyeraGrad.hpp"
```

Now, you can use all the features of this small engine.

@note _Shkyera Grad_ is tested in C++17. Make sure your compiler supports this version.

## Scalars

Internally, _Shkyera Grad_ **always** operates on individual scalars. For most purposes, you do not need to deal with them directly, but it's nice to understand how they work. Each scalar is wrapped inside a `Value` class. However, you should never instantiate objects of this type yourself. Instead, you should use the provided interface in the following way.

```{.cpp}
// Creates a floating-point scalar
ValuePtr<float> a = Value<float>::create(5.2);
ValuePtr<Type::f32> a = Value<Type::f32>::create(5.2);
ValuePtr<Type::float32> a = Value<Type::float32>::create(5.2);
auto a = Value<float>::create(5.2);
auto a = Value<Type::float32>::create(5.2);
auto a = Value<Type::float64>::create(5.2);
auto a = Val32::create(5.2);

// Now with higher precision!
ValuePtr<Type::float64> b = Value<double>::create(6.9);
auto b = Value<Type::f64>::create(6.9);
auto b = Val64::create(6.9);

// You can also use integers, no clue why, but go for it!
auto c = Value<int>::create(7);
```

You can also perform various operations directly on scalars!

```{.cpp}
using T = Type::float32;

auto a = Value<T>::create(2.1);
auto b = Value<T>::create(3.7);
auto c = a - b;
auto d = a * b / c;
c = d->log();
auto e = (a + b - c)->pow(d);
```

@note Check out the cheatsheet for the list of all operations.

The magic behind the _Shkyera Grad_ is that it keeps track of all the operations, so that you can later calculate the derivatives of your expression.

```{.cpp}
auto a = Value<T>::create(2.0);
auto b = Value<T>::create(3.0);
auto c = a * b;

c->getValue();                  // c = 6.0
c->backward();                  // Calculate the gradients of the expression

a->getGradient();               // dc/da = 3.0
b->getGradient();               // dc/db = 2.0
```

If you want some refreshment on derivatives, check out [this wonderful video](https://www.youtube.com/watch?v=9vKqVkMQHKk).

## Vector

Multiple scalars can be grouped together in a `Vector` to simplify operating on them. Input to any `Module` (more on them later) is a `Vector`. This abstraction provides some functionality that allows you to compute, for example a dot product.

```{.cpp}
// The easiest way to create e Vector
auto a = Vector<T>::of(1, 2, 3);

// A bit more annoying way to create a Vector
auto b = Vector<T>::of({1, 2, 3});

// The hard way to create a Vector
auto c = Vector<T>(Value<T>::create(2), Value<T>::create(3), Value<T>::create(4));

// You can access elements in a vector
auto d = Vector<T>::of({a[0]*b[0], a[1]*b[1], a[2]*b[2]});

// And even iterate over it
for(auto &entry : d)
    std::cout << entry << std::endl; // prints: 2 6 12

auto e = b.dot(c)       // c = 1 * 2 + 2 * 3 + 3 * 4 = 20
e->backward();          // You can compute of this result since it's a scalar!
```

`Vectors` are very useful since this is the way both the input and output data is represented. Each sample consits of an input `Vector` and a target output `Vector`.

## Sequential

Nice! You got the basics! Let's build a network. The best way to create a model is through the use of the `Sequential` interface. Each function that transforms an input `Vector` into some output `Vector` is implemented as a `Module`. This includes neural layers as well as activation functions. Hey, even `Sequential` is a `Module`. This allows for creating complex strctures while using a common, simple interface.

You can create your first neural network using `SequentialBuilder` in the following way.

```{.cpp}
auto network = SequentialBuilder<T>::begin()
               .add(Linear<T>::create(2, 15))       // Adds a layer with 2 inputs and 15 outputs
               .add(ReLU<T>::create())              // Adds a ReLU activation function
               .add(Linear32::create(15, 10))       // You can use {Layer}32 or {Layer}64 macros
               .add(Sigmoid32::create())            // More fancy activation :0
               .add(Dropout32::create(10, 2, 0.5))  // We use the dropout rate of 0.5
               .build();                            // Don't forget to actually build your network
```

@warning Remember that subsequent layers have to have matching input and output sizes.

@note For the full list of available layers and activation functions, check out the Cheat Sheet.

## Training

To train our network, we need to define an `Optimizer` that will optimizer the parameters as well as the `Loss` function that we will minimize. _Shkyera Grad_ comes with a set of well-known optimizers and loss functions. Again, check out the Cheat Sheet for a complete list.

```{.cpp}
// Simple stochastic gradient descent optimizer with 0.01 learning rate
auto optimizer = Optimizer<T>(network->parameters(), 0.01);

// Stochastic gradient descent, but with momentum of 0.99!
// If you provide no parameter for momentum, it defaults to 0.9
auto betterOptimizer = SGD32(network->parameters(), 0.01, 0.99);

// Recommended optimizer: Adam as described in the original paper
// Again, 0.01 is the learning rate.
auto awesomeOptimizer = Adam32(network->parameters(), 0.01);

// By default, it comes with the recommended parameters,
// but they can be changed if you feel like it in the following way:
auto awesomeCustomOptimizer = Adam32(network->parameters(), 0.01, beta1, beta2, epsilon);
```

Here's a list of some available `Loss` functions:

```{.cpp}
Loss::MAE<T>            // Mean Absolute Error
Loss::MSE<T>            // Mean Squared Error
Loss::CrossEntropy<T>   // Cross Entropy Loss - good for classification
```

They are implemented as lambda functions, not as objects, so they do not need to be instantiated.

## Learning XOR

XOR (Exclusive OR) is a simple Boolean function that maps two values two one:

| X1  | X2  | Result |
| --- | --- | ------ |
| 0   | 0   | 0      |
| 0   | 1   | 1      |
| 1   | 0   | 1      |
| 1   | 1   | 0      |

### Let's define our dataset.

Here, we basically pase the table above into `Vector`s.

```{.cpp}
std::vector<Vec32> xs;
std::vector<Vec32> ys;

// ---------- INPUT ----------- | -------- OUTPUT --------- //
xs.push_back(Vec32::of(0, 0)); ys.push_back(Vec32::of(0));
xs.push_back(Vec32::of(1, 0)); ys.push_back(Vec32::of(1));
xs.push_back(Vec32::of(0, 1)); ys.push_back(Vec32::of(1));
xs.push_back(Vec32::of(1, 1)); ys.push_back(Vec32::of(0));
```

### Neural Network

We define a simple neural network to predict this function. Our network has a total of three layers. It is a bit of an overkill for this task, but we will use it for learning purposes.

```{.cpp}
auto network = SequentialBuilder<Type::float32>::begin()
                .add(Linear32::create(2, 15))
                .add(ReLU32::create())
                .add(Linear32::create(15, 5))
                .add(ReLU32::create())
                .add(Linear32::create(5, 1))
                .add(Sigmoid32::create())
                .build();
```

### Training Loop

Now, we just need to specify the optimizer and the loss function we want to use:

```{.cpp}
auto optimizer = Adam32(network->parameters(), 0.05);
auto lossFunction = Loss::MSE<T>;
```

We train our model for 100 epochs. After each epoch, we pring the average loss.

```{.cpp}
for (size_t epoch = 0; epoch < 100; epoch++) {              // We train for 100 epochs
    auto epochLoss = Val32::create(0);

    optimizer.reset();                                      // Reset the gradients
    for (size_t sample = 0; sample < xs.size(); ++sample) { // We go through each sample
        Vec32 pred = network->forward(xs[sample]);          // We get some prediction
        auto loss = lossFunction(pred, ys[sample]);         // And calculate its error

        epochLoss = epochLoss + loss;                       // Store the loss for feedback
    }
    optimizer.step();                                       // Update the parameters

    auto averageLoss = epochLoss / Val32::create(xs.size());
    std::cout << "Epoch: " << epoch + 1 << " Loss: " << averageLoss->getValue() << std::endl;
}
```

### Verifying the results

After the training, let's inspect how our network behaves.

```{.cpp}
for (size_t sample = 0; sample < xs.size(); ++sample) {         // Go through each example
    Vec32 pred = network->forward(xs[sample]);                  // Predict result
    std::cout << xs[sample] << " -> " << pred << "\t| True: " << ys[sample] << std::endl;
}
```

In case you got lost along the way, check out the `examples/xor_regression.cpp` file. It contains the exact same code and is ready to run :)

### Results

Nice! After compiling and running this code (make sure to use C++17), you should see something like this:

```
Epoch: 1 Loss: 0.263062
Epoch: 2 Loss: 0.211502
(...)
Epoch: 99 Loss: 0.000222057
Epoch: 100 Loss: 0.00020191
Vector(size=2, data={Value(data=0) Value(data=0) }) -> Value(data=0.0191568)    | True: Value(data=0)
Vector(size=2, data={Value(data=1) Value(data=0) }) -> Value(data=0.99998)      | True: Value(data=1)
Vector(size=2, data={Value(data=0) Value(data=1) }) -> Value(data=0.999984)     | True: Value(data=1)
Vector(size=2, data={Value(data=1) Value(data=1) }) -> Value(data=0.0191568)    | True: Value(data=0)
```

WOW! The network actually learned the XOR function.

This is it. You should have enough knowledge to start experimenting with _Shkyera Engine_. Let us know on GitHub what do you think about this project :)

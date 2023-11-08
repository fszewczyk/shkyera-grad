## Examples

To compile an example, simply run the following command:

```
g++ --std=c++17 xor_nn.cpp
```

Remember to replace the file name with the appropriate name :)

## Scalars

Provides a brief overview of operating on scalars.

## XOR Neural Network

Small neural network that learns the xor function. In particular, given a vector of two values, it predicts a single value according to the XOR function. The training runs with a learning rate of 0.1 for 100 epochs using MSE loss.

After running this example, the output should look somewhat like this:

```
Epoch: 1 Loss: 1.57581
Epoch: 2 Loss: 1.46817
(...)
Epoch: 99 Loss: 0.0386917
Epoch: 100 Loss: 0.0371898
Vector(size=2, data={Value(data=0) Value(data=0) }) -> Value(data=0.115728)| True: Value(data=0)
Vector(size=2, data={Value(data=1) Value(data=0) }) -> Value(data=0.93215) | True: Value(data=1)
Vector(size=2, data={Value(data=0) Value(data=1) }) -> Value(data=0.937625)| True: Value(data=1)
Vector(size=2, data={Value(data=1) Value(data=1) }) -> Value(data=0.115728)| True: Value(data=0)
```

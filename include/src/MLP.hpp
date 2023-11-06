#pragma once

#include "Activation.hpp"
#include "Layer.hpp"
#include "Module.hpp"
#include "Vector.hpp"

namespace shkyera {

template <typename T> class MLP : public Module<T> {
  private:
    std::vector<Layer<T>> _layers;

  public:
    MLP(size_t input, std::vector<size_t> sizes, std::vector<Activation::Function<T>> activations);

    virtual Vector<T> operator()(const Vector<T> &x) const override;
    virtual std::vector<ValuePtr<T>> parameters() const override;
};

template <typename T>
MLP<T>::MLP(size_t input, std::vector<size_t> sizes, std::vector<Activation::Function<T>> activations) {
    if (sizes.size() == 0)
        throw std::invalid_argument("MLP must be passed at least one size: an output size.");
    if (sizes.size() != activations.size())
        throw std::invalid_argument(
            "MLP constructor must be passed the vector of sizes of the same size as vector of activations. Sizes are " +
            std::to_string(sizes.size()) + " and " + std::to_string(activations.size()) + ".");

    _layers.reserve(sizes.size());
    _layers.emplace_back(Layer<T>(input, sizes[0], activations[0]));

    for (size_t i = 1; i < sizes.size(); ++i) {
        _layers.emplace_back(Layer<T>(sizes[i - 1], sizes[i], activations[i]));
    }
}

template <typename T> Vector<T> MLP<T>::operator()(const Vector<T> &x) const {
    Vector<T> out = _layers[0](x);

    std::for_each(_layers.begin() + 1, _layers.end(), [&out](Layer<T> layer) { out = layer(out); });

    return out;
}

template <typename T> std::vector<ValuePtr<T>> MLP<T>::parameters() const {
    std::vector<ValuePtr<T>> params;
    for (const Layer<T> &l : _layers) {
        std::vector<ValuePtr<T>> layerParams = l.parameters();
        params.insert(params.end(), layerParams.begin(), layerParams.end());
    }

    return params;
}

} // namespace shkyera

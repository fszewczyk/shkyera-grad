/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <exception>
#include <vector>

namespace shkyera {

template <typename T, typename U> class Dataset {
  private:
    std::vector<T> _inputs;
    std::vector<U> _outputs;

  public:
    Dataset() = default;
    Dataset(const std::vector<T> &inputs, const std::vector<T> &outputs);

    void addSample(T input, U output);

    size_t size() const;

    std::pair<T, U> operator[](size_t index) const;

    class ConstIterator {
      private:
        size_t _index;
        const Dataset<T, U> &_dataset;

      public:
        ConstIterator(size_t index, const Dataset<T, U> &dataset);

        const std::pair<T, U> operator*();
        ConstIterator &operator++();
        bool operator!=(const ConstIterator &other);
    };

    ConstIterator begin() const;
    ConstIterator end() const;
};

template <typename T, typename U> Dataset<T, U>::Dataset(const std::vector<T> &inputs, const std::vector<T> &outputs) {
    if (inputs.size() != outputs.size())
        throw std::invalid_argument(
            "To create a dataset, you have to pass the same amount of inputs and outputs. You passed " +
            std::to_string(inputs.size()) + " inputs and " + std::to_string(outputs.size()) + " outputs.");

    _inputs = inputs;
    _outputs = outputs;
}

template <typename T, typename U> void Dataset<T, U>::addSample(T input, U output) {
    _inputs.push_back(input);
    _outputs.push_back(output);
}

template <typename T, typename U> size_t Dataset<T, U>::size() const { return _inputs.size(); }

template <typename T, typename U> std::pair<T, U> Dataset<T, U>::operator[](size_t index) const {
    if (index > _inputs.size())
        throw std::invalid_argument("While trying to access Dataset, the provided index " +
                                    std::to_string(_inputs.size()) + " was too large for a dataset of size " +
                                    std::to_string(size()));
    return {_inputs[index], _outputs[index]};
}

template <typename T, typename U>
Dataset<T, U>::ConstIterator::ConstIterator(size_t index, const Dataset<T, U> &dataset)
    : _index(index), _dataset(dataset) {}

template <typename T, typename U> const std::pair<T, U> Dataset<T, U>::ConstIterator::operator*() {
    return _dataset[_index];
}

template <typename T, typename U> typename Dataset<T, U>::ConstIterator &Dataset<T, U>::ConstIterator::operator++() {
    ++_index;
    return *this;
}

template <typename T, typename U> bool Dataset<T, U>::ConstIterator::operator!=(const ConstIterator &other) {
    return _index != other._index;
}

template <typename T, typename U> typename Dataset<T, U>::ConstIterator Dataset<T, U>::begin() const {
    return ConstIterator(0, *this);
}

template <typename T, typename U> typename Dataset<T, U>::ConstIterator Dataset<T, U>::end() const {
    return ConstIterator(_inputs.size(), *this);
}

} // namespace shkyera

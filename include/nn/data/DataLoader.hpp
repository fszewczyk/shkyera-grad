/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include "Dataset.hpp"

namespace shkyera {

template <typename T, typename U> class DataLoader {
  private:
    const Dataset<T, U> &_dataset;
    size_t _batchSize;
    bool _shuffle;

  public:
    DataLoader(const Dataset<T, U> &dataset, size_t batchSize = 4, bool shuffle = false);

    size_t getTotalBatches() const;

    class ConstIterator {
      private:
        std::vector<size_t> _order;
        size_t _index;

        const DataLoader<T, U> &_dataLoader;

      public:
        ConstIterator(size_t index, const DataLoader<T, U> &dataLoader);

        std::pair<std::vector<T>, std::vector<U>> operator*();
        ConstIterator &operator++();
        bool operator!=(const ConstIterator &other);
    };

    ConstIterator begin() const;
    ConstIterator end() const;
};

template <typename T, typename U>
DataLoader<T, U>::DataLoader(const Dataset<T, U> &dataset, size_t batchSize, bool shuffle)
    : _dataset(dataset), _batchSize(batchSize), _shuffle(shuffle) {}

template <typename T, typename U> size_t DataLoader<T, U>::getTotalBatches() const {
    size_t batches = _dataset.size() / _batchSize;
    if (_dataset.size() % _batchSize != 0)
        batches++;
    return batches;
}

template <typename T, typename U>
DataLoader<T, U>::ConstIterator::ConstIterator(size_t index, const DataLoader<T, U> &dataLoader)
    : _index(index), _dataLoader(dataLoader) {
    _order.resize(_dataLoader._dataset.size(), 0);

    std::iota(_order.begin(), _order.end(), 0);
    if (_dataLoader._shuffle)
        utils::shuffle(_order);
}

template <typename T, typename U>
std::pair<std::vector<T>, std::vector<U>> DataLoader<T, U>::ConstIterator::operator*() {
    size_t beginIndex = _index;
    size_t endIndex = std::min(_index + _dataLoader._batchSize, _dataLoader._dataset.size());

    std::vector<T> inputs(endIndex - beginIndex);
    std::vector<U> outputs(endIndex - beginIndex);

    for (size_t i = beginIndex; i < endIndex; ++i) {
        auto [in, out] = _dataLoader._dataset[_order[i]];
        inputs[i - beginIndex] = in;
        outputs[i - beginIndex] = out;
    }

    return {inputs, outputs};
}

template <typename T, typename U>
typename DataLoader<T, U>::ConstIterator &DataLoader<T, U>::ConstIterator::operator++() {
    _index += _dataLoader._batchSize;
    _index = std::min(_index, _dataLoader._dataset.size());
    return *this;
}

template <typename T, typename U> bool DataLoader<T, U>::ConstIterator::operator!=(const ConstIterator &other) {
    return _index != other._index;
}

template <typename T, typename U> typename DataLoader<T, U>::ConstIterator DataLoader<T, U>::begin() const {
    return ConstIterator(0, *this);
}

template <typename T, typename U> typename DataLoader<T, U>::ConstIterator DataLoader<T, U>::end() const {
    return ConstIterator(_dataset.size(), *this);
}

} // namespace shkyera

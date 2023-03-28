#pragma once

#include "Dataset.hpp"
#include "Layer.hpp"

namespace st {

class Network {
  public:
    Network(Dataset &dataset, std::vector<size_t> hiddenLayersSizes,
            std::vector<ACTIVATION> hiddenLayersActivations = {});

    void train(double learningRate = 0.01, size_t batchSize = 64, size_t epochs = 10);

  private:
    Vector<double> feedForward(Vector<double> input);
    void backpropagate(double learningRate);

    bool m_softmaxOutput;

    Dataset &m_dataset;
    std::vector<Layer> m_layers;
};

Network::Network(Dataset &dataset, std::vector<size_t> hiddenLayersSizes,
                 std::vector<ACTIVATION> hiddenLayersActivations)
    : m_dataset(dataset) {
    size_t inputSize = m_dataset.getInputSize();
    size_t outputSize = m_dataset.getOutputSize();

    hiddenLayersSizes.insert(hiddenLayersSizes.begin(), inputSize);
    hiddenLayersSizes.push_back(outputSize);

    for (size_t i = 0; i < hiddenLayersSizes.size() - 1; ++i) {
        ACTIVATION activation = hiddenLayersActivations.size() > i ? RELU : hiddenLayersActivations[i];
        m_layers.push_back(Layer(hiddenLayersSizes[i], hiddenLayersSizes[i + 1], activation));
    }
}

void Network::train(double learningRate, size_t batchSize, size_t epochs) {
    std::vector<Data> &data = m_dataset.getData();
    for (size_t epoch = 0; epoch < epochs; ++epoch) {
        int globalIndex = 0;

        std::vector<Vector<double>> trueValues;
        std::vector<Vector<double>> calculatedValues;
        for (size_t sample = 0; sample < batchSize && globalIndex < data.size(); ++sample) {
            trueValues.push_back(data[globalIndex].getLabel());
            calculatedValues.push_back(feedForward(data[globalIndex].getData()));
            globalIndex++;
        }
    }
}

void Network::backpropagate(double learningRate) {}

Vector<double> Network::feedForward(Vector<double> input) {
    std::vector<Vector<double>> intermediateResults;
    intermediateResults.push_back(input);
    for (size_t i = 0; i < m_layers.size(); ++i) {
        intermediateResults.push_back(m_layers[i].feedForward(intermediateResults[i]));
    }

    if (m_dataset.getLabelType() == CATEGORICAL) {
        intermediateResults.back().softmax();
    }

    return intermediateResults.back();
}

} // namespace st
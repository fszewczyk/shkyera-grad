#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "math/Utils.hpp"
#include "math/Vector.hpp"

namespace st {

enum LABEL_TYPE { CONTINUOUS, CATEGORICAL };

class Data {
  public:
    Data() = default;
    Data(Vector<double> label, Vector<double> data);

    void normalize(size_t index, double min, double max);

    Vector<double> getLabel() const;
    Vector<double> getData() const;

    void resizeLabel(size_t size);

  private:
    Vector<double> m_label;
    Vector<double> m_data; //< TODO: Implement tensor
};

class Dataset {
  public:
    Dataset() = default;
    Dataset(std::vector<Data> data);
    Dataset(std::string path, LABEL_TYPE labelType = CATEGORICAL, bool containsHeaders = true);

    void shuffle();
    std::vector<Data> head(size_t size = 5) const;

    void loadDataset(std::string path, LABEL_TYPE labelType = CATEGORICAL, bool containsHeaders = true);
    std::pair<Dataset, Dataset> splitIntoTrainAndTest(float proportion = 0.8);

    std::vector<Data> &getData();

    size_t getInputSize() const;
    size_t getOutputSize() const;

    bool getLabelType() const;

    inline static bool NORMALIZE_FLOATS = true;

  private:
    Data readLine(std::string &line);
    void oneHotEncodeLabels();
    void normalize();

    LABEL_TYPE m_labelType;

    std::unordered_map<std::string, int> categoryToIndex;
    std::unordered_map<int, std::string> indexToCategory;
    std::vector<Data> m_data;
};

Data::Data(Vector<double> label, Vector<double> data) {
    m_label = label;
    m_data = data;
}

void Data::normalize(size_t i, double min, double max) {
    m_data(i) -= min;
    m_data(i) /= std::max(0.001, (max - min));
}

Vector<double> Data::getLabel() const { return m_label; }
Vector<double> Data::getData() const { return m_data; }

void Data::resizeLabel(size_t size) { m_label.resize(size); }

inline std::ostream &operator<<(std::ostream &out, const Data &data) {
    return out << "Data:{Label:" << data.getLabel() << ", Data: " << data.getData() << "}";
}

Dataset::Dataset(std::string path, LABEL_TYPE labelType, bool containsHeaders) {
    loadDataset(path, labelType, containsHeaders);
}

Dataset::Dataset(std::vector<Data> data) { m_data = data; }

void Dataset::shuffle() { utils::shuffle(m_data); }

std::vector<Data> Dataset::head(size_t size) const {
    size = std::min(size, m_data.size());
    std::vector<Data> h(m_data.begin(), m_data.begin() + size);

    return h;
}

void Dataset::loadDataset(std::string path, LABEL_TYPE labelType, bool containsHeaders) {
    std::cerr << "Loading dataset: " << path << '\n';
    m_labelType = labelType;

    std::fstream file(path, std::ios::in);

    if (!file.is_open())
        throw std::invalid_argument("Cannot read file " + path);

    std::string line;
    if (containsHeaders)
        std::getline(file, line);

    std::vector<Data> dataset;
    while (std::getline(file, line)) {
        m_data.push_back(readLine(line));
    }

    if (labelType == CATEGORICAL)
        oneHotEncodeLabels();

    normalize();

    std::cerr << "Loaded dataset: " << path << '\n';
}

Data Dataset::readLine(std::string &line) {
    bool setLabel = false;
    Vector<double> label;
    Vector<double> data;

    std::string word;
    std::stringstream stream(line);

    while (std::getline(stream, word, ',')) {
        if (!setLabel) {
            if (m_labelType == CONTINUOUS)
                label = {1, utils::convert<double>(word)};
            else if (m_labelType == CATEGORICAL) {
                if (categoryToIndex.find(word) == categoryToIndex.end()) {
                    categoryToIndex.insert({word, categoryToIndex.size()});
                    indexToCategory.insert({categoryToIndex.size() - 1, word});
                }

                label = Vector<double>::zeros(categoryToIndex.size());
                label(categoryToIndex[word]) = 1;
            }

            setLabel = true;
        } else {
            data.append(utils::convert<double>(word));
        }
    }

    return Data(label, data);
}

std::vector<Data> &Dataset::getData() { return m_data; }

void Dataset::oneHotEncodeLabels() {
    for (auto &d : m_data) {
        d.resizeLabel(categoryToIndex.size());
    }
}

void Dataset::normalize() {
    if (m_data.empty())
        return;

    std::vector<double> dataMinimums(m_data[0].getData().getSize(), std::numeric_limits<double>::max());
    std::vector<double> dataMaximums(m_data[0].getData().getSize(), std::numeric_limits<double>::lowest());

    for (auto d : m_data) {
        auto data = d.getData();
        for (size_t i = 0; i < data.getSize(); ++i) {
            dataMinimums[i] = std::min(dataMinimums[i], data[i]);
            dataMaximums[i] = std::max(dataMaximums[i], data[i]);
        }
    }

    for (auto &d : m_data) {
        for (size_t i = 0; i < d.getData().getSize(); ++i) {
            d.normalize(i, dataMinimums[i], dataMaximums[i]);
        }
    }
}

size_t Dataset::getInputSize() const {
    if (m_data.empty())
        throw std::runtime_error("Accessing data size, while the data is empty");

    return m_data[0].getData().getSize();
}

size_t Dataset::getOutputSize() const {
    if (m_data.empty())
        throw std::runtime_error("Accessing data size, while the data is empty");

    return m_data[0].getLabel().getSize();
}

bool Dataset::getLabelType() const { return m_labelType; }

std::pair<Dataset, Dataset> Dataset::splitIntoTrainAndTest(float proportion) {
    size_t cutoff = static_cast<size_t>(proportion * m_data.size());

    std::vector<Data> train(m_data.begin(), m_data.begin() + cutoff);
    std::vector<Data> test(m_data.begin() + cutoff, m_data.end());

    return {Dataset(train), Dataset(test)};
}

} // namespace st

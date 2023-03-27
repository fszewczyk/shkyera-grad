#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "math/Utils.hpp"
#include "math/Vector.hpp"

namespace st {

template <typename T, typename U> class Data {
  public:
    Data() = default;
    Data(T label, Vector<U> data);

    void normalize();

    T getLabel() const;
    Vector<U> getData() const;

    static Data<T, U> readLine(std::string &line, bool headers = false);

  private:
    T m_label;
    Vector<U> m_data; //< TODO: Implement tensor
};

template <typename T, typename U> class Dataset {
  public:
    Dataset() = default;
    Dataset(std::vector<Data<T, U>> data);
    Dataset(std::string path, bool containsHeaders = true);

    void shuffle();
    std::vector<Data<T, U>> head(size_t size = 5) const;

    void loadDataset(std::string path, bool containsHeaders = true);
    std::pair<Dataset, Dataset> splitIntoTrainAndTest(float proportion = 0.8);

    inline static bool NORMALIZE_FLOATS = true;

  private:
    std::vector<std::string> m_headers;
    std::vector<Data<T, U>> m_data;
};

template <typename T, typename U> Data<T, U>::Data(T label, Vector<U> data) {
    m_label = label;
    m_data = data;
}

template <typename T, typename U> void Data<T, U>::normalize() { m_data /= m_data.max(); }
template <typename T, typename U> T Data<T, U>::getLabel() const { return m_label; }
template <typename T, typename U> Vector<U> Data<T, U>::getData() const { return m_data; }

template <typename T, typename U> Data<T, U> Data<T, U>::readLine(std::string &line, bool headers) {
    bool setLabel = false;
    T label;
    Vector<U> data;

    std::string word;
    std::stringstream stream(line);

    while (std::getline(stream, word, ',')) {
        if (!setLabel && !headers) {
            label = utils::convert<T>(word);
            setLabel = true;
        } else {
            data.append(utils::convert<U>(word));
        }
    }

    if constexpr (std::is_floating_point<U>::value) {
        if (Dataset<T, U>::NORMALIZE_FLOATS)
            data /= data.max();
    }

    return Data(label, data);
}

template <typename T, typename U> inline std::ostream &operator<<(std::ostream &out, const Data<T, U> &data) {
    return out << "DataRow:{Label:" << data.getLabel() << ", Data: " << data.getData() << "}";
}

template <typename T, typename U> Dataset<T, U>::Dataset(std::string path, bool containsHeaders) {
    loadDataset(path, containsHeaders);
}

template <typename T, typename U> Dataset<T, U>::Dataset(std::vector<Data<T, U>> data) { m_data = data; }

template <typename T, typename U> void Dataset<T, U>::shuffle() { utils::shuffle(m_data); }

template <typename T, typename U> std::vector<Data<T, U>> Dataset<T, U>::head(size_t size) const {
    size = std::min(size, m_data.size());
    std::vector<Data<T, U>> h(m_data.begin(), m_data.begin() + size);

    return h;
}

template <typename T, typename U> void Dataset<T, U>::loadDataset(std::string path, bool containsHeaders) {
    std::fstream file(path, std::ios::in);

    if (!file.is_open())
        throw std::invalid_argument("Cannot read file " + path);

    std::string line;
    if (containsHeaders) {
        m_headers.clear();
        std::getline(file, line);
        m_headers = Data<std::string, std::string>::readLine(line, true).getData().getValues();
    }

    std::vector<Data<T, U>> dataset;
    while (std::getline(file, line)) {
        m_data.push_back(Data<T, U>::readLine(line));
    }
}

template <typename T, typename U>
std::pair<Dataset<T, U>, Dataset<T, U>> Dataset<T, U>::splitIntoTrainAndTest(float proportion) {
    size_t cutoff = static_cast<size_t>(proportion * m_data.size());

    std::vector<Data<T, U>> train(m_data.begin(), m_data.begin() + cutoff);
    std::vector<Data<T, U>> test(m_data.begin() + cutoff, m_data.end());

    return {Dataset<T, U>(train), Dataset<T, U>(test)};
}

} // namespace st

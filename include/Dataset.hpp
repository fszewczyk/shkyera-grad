#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "math/Utils.hpp"
#include "math/Vector.hpp"

namespace st {

template <typename T, typename U> class Data {
  public:
    Data();
    Data(T label, Vector<U> data);

    T getLabel() const;
    Vector<U> getData() const;

    static std::vector<Data> loadDataset(std::string path, Vector<std::string> &headers, bool containsHeaders = true);
    static std::pair<Data, Data> splitIntoTrainAndTest(float proportion = 0.8);

  private:
    static Data readLine(std::string &line, bool headers = false);

    T m_label;
    Vector<U> m_data; //< TODO: Implement tensor
};

template <typename T, typename U> Data<T, U>::Data(T label, Vector<U> data) {
    m_label = label;
    m_data = data;
}

template <typename T, typename U> T Data<T, U>::getLabel() const { return m_label; }
template <typename T, typename U> Vector<U> Data<T, U>::getData() const { return m_data; }

template <typename T, typename U>
std::vector<Data<T, U>> Data<T, U>::loadDataset(std::string path, Vector<std::string> &headers, bool containsHeaders) {
    std::fstream file(path, std::ios::in);

    if (!file.is_open())
        throw std::invalid_argument("Cannot read file " + path);

    std::string line;
    if (containsHeaders) {
        headers.clear();
        std::getline(file, line);
        headers = Data<std::string, std::string>::readLine(line, true).getData();
    }

    std::vector<Data<T, U>> dataset;
    while (std::getline(file, line)) {
        dataset.push_back(readLine(line));
    }

    return dataset;
}

template <typename T, typename U> Data<T, U> Data<T, U>::readLine(std::string &line, bool headers) {
    bool setLabel = false;
    T label;
    Vector<U> data;

    std::string word;
    std::stringstream stream(line);

    while (std::getline(stream, word, ',')) {
        if (!setLabel && !headers) {
            label = convert<T>(word);
            setLabel = true;
        } else {
            data.append(convert<U>(word));
        }
    }

    return Data(label, data);
}

template <typename T, typename U>
std::pair<Data<T, U>, Data<T, U>> Data<T, U>::splitIntoTrainAndTest(float proportion) {}

} // namespace st

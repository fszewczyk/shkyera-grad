#pragma once

#include <fstream>
#include <iostream>

#include "Vector.hpp"

namespace st {

template <typename T, typename U> class Data {
  public:
    Data();
    Data(T label, Vector<U> data);

    T getLabel() const;
    Vector<U> getData() const;

    static std::vector<Data> loadDataset(std::string path, std::vector<std::string> &headers,
                                         bool containsHeaders = true);
    static std::pair<Data, Data> splitIntoTrainAndTest(float proportion = 0.8);

  private:
    static Data readLine(std::fstream &file, bool headers = false);

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
std::vector<Data<T, U>> Data<T, U>::loadDataset(std::string path, std::vector<std::string> &headers,
                                                bool containsHeaders) {
    std::fstream file(path, std::ios::in);

    if (containsHeaders) {
        headers.clear();
        headers = Data<std::string, std::string>::readLine(file, true).getData();
    }
}

template <typename T, typename U> Data<T, U> Data<T, U>::readLine(std::fstream &file, bool headers) {
    T label;
    std::vector<U> data;
}

template <typename T, typename U>
std::pair<Data<T, U>, Data<T, U>> Data<T, U>::splitIntoTrainAndTest(float proportion) {}

} // namespace st
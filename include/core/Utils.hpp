#include <sstream>
#include <string>

template <typename T> T convert(const std::string &str) {
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}
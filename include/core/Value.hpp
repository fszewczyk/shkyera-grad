/**
 * Copyright © 2023 Franciszek Szewczyk. None of the rights reserved.
 * This code is released under the Beerware License. If you find this code useful or you appreciate the work, you are
 * encouraged to buy the author a beer in return.
 * Contact the author at szewczyk.franciszek02@gmail.com for inquiries and support.
 */

#pragma once

#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <stack>
#include <unordered_set>
#include <vector>

#include "Type.hpp"
#include "Utils.hpp"

namespace shkyera {

template <typename T> class Optimizer;
template <typename T> class Adam;
template <typename T> class AdaMax;
template <typename T> class SGD;
template <typename T> class NAG;

template <typename T> class Value;
template <typename T> using ValuePtr = std::shared_ptr<Value<T>>;

using Val32 = Value<Type::float32>;
using Val64 = Value<Type::float64>;

template <typename T> class Value : public std::enable_shared_from_this<Value<T>> {
  private:
    T _data = 0;
    T _gradient = 0;
    std::vector<ValuePtr<T>> _children = {};
    std::function<void()> _backward = []() {};

    Value(T data);

    std::vector<ValuePtr<T>> topologicalSort();

    inline static double topoSortTime = 0;

  public:
    friend class Optimizer<T>;
    friend class Adam<T>;
    friend class AdaMax<T>;
    friend class SGD<T>;
    friend class NAG<T>;

    static ValuePtr<T> create(T data);

    void backward();
    T getValue();
    T getGradient();

    static double getTopoTime() { return topoSortTime; }

    ValuePtr<T> tanh();
    ValuePtr<T> relu();
    ValuePtr<T> sigmoid();
    ValuePtr<T> exp();
    ValuePtr<T> log();
    ValuePtr<T> pow(ValuePtr<T> exponent);

    template <typename U> friend ValuePtr<U> operator+(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend ValuePtr<U> operator-(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend ValuePtr<U> operator*(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend ValuePtr<U> operator/(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend ValuePtr<U> operator-(ValuePtr<U> a);

    template <typename U> friend bool operator>(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend bool operator>=(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend bool operator<(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend bool operator<=(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend bool operator==(ValuePtr<U> a, ValuePtr<U> b);
    template <typename U> friend bool operator!=(ValuePtr<U> a, ValuePtr<U> b);

    template <typename U> friend std::ostream &operator<<(std::ostream &os, const ValuePtr<U> &value);
};

template <typename T> Value<T>::Value(T data) : _data(data) {}

template <typename T> ValuePtr<T> Value<T>::create(T data) { return std::shared_ptr<Value<T>>(new Value<T>(data)); }

template <typename T> T Value<T>::getValue() { return _data; }

template <typename T> T Value<T>::getGradient() { return _gradient; }

template <typename T> ValuePtr<T> operator+(ValuePtr<T> a, ValuePtr<T> b) {
    ValuePtr<T> result = Value<T>::create(a->_data + b->_data);
    result->_children = {a, b};
    result->_backward = [a, b, result]() {
        a->_gradient += result->_gradient;
        b->_gradient += result->_gradient;
    };

    return result;
}

template <typename T> ValuePtr<T> operator-(ValuePtr<T> a, ValuePtr<T> b) { return a + (-b); }

template <typename T> ValuePtr<T> operator*(ValuePtr<T> a, ValuePtr<T> b) {
    ValuePtr<T> result = Value<T>::create(a->_data * b->_data);
    result->_children = {a, b};
    result->_backward = [a, b, result]() {
        a->_gradient += b->_data * result->_gradient;
        b->_gradient += a->_data * result->_gradient;
    };

    return result;
}

template <typename T> ValuePtr<T> operator/(ValuePtr<T> a, ValuePtr<T> b) { return a * (b->pow(Value<T>::create(-1))); }

template <typename T> ValuePtr<T> operator-(ValuePtr<T> a) { return Value<T>::create(-1) * a; }

template <typename T> bool operator<(ValuePtr<T> a, ValuePtr<T> b) { return a->getValue() < b->getValue(); }
template <typename T> bool operator<=(ValuePtr<T> a, ValuePtr<T> b) { return a->getValue() <= b->getValue(); }
template <typename T> bool operator>(ValuePtr<T> a, ValuePtr<T> b) { return a->getValue() > b->getValue(); }
template <typename T> bool operator>=(ValuePtr<T> a, ValuePtr<T> b) { return a->getValue() >= b->getValue(); }
template <typename T> bool operator==(ValuePtr<T> a, ValuePtr<T> b) { return a->getValue() == b->getValue(); }
template <typename T> bool operator!=(ValuePtr<T> a, ValuePtr<T> b) { return a->getValue() != b->getValue(); }

template <typename T> ValuePtr<T> Value<T>::tanh() {
    auto thisValue = this->shared_from_this();

    ValuePtr<T> result = Value<T>::create((std::exp(2 * thisValue->_data) - 1) / (std::exp(2 * thisValue->_data) + 1));
    result->_children = {thisValue};
    result->_backward = [thisValue, result]() {
        thisValue->_gradient += (1 - (result->_data * result->_data)) * result->_gradient;
    };

    return result;
}

template <typename T> ValuePtr<T> Value<T>::sigmoid() {
    auto thisValue = this->shared_from_this();

    ValuePtr<T> result = Value<T>::create(1 / (std::exp(-thisValue->_data) + 1));
    result->_children = {thisValue};
    result->_backward = [thisValue, result]() {
        thisValue->_gradient += result->_data * (1 - result->_data) * result->_gradient;
    };

    return result;
}

template <typename T> ValuePtr<T> Value<T>::relu() {
    auto thisValue = this->shared_from_this();

    ValuePtr<T> result = Value<T>::create(_data > 0 ? _data : 0);
    result->_children = {thisValue};
    result->_backward = [thisValue, result]() {
        thisValue->_gradient += (result->_data > 0 ? 1 : 0) * result->_gradient;
    };

    return result;
}

template <typename T> ValuePtr<T> Value<T>::exp() {
    auto thisValue = this->shared_from_this();

    ValuePtr<T> result = Value<T>::create(std::exp(_data));
    result->_children = {thisValue};
    result->_backward = [thisValue, result]() { thisValue->_gradient += result->_data * result->_gradient; };

    return result;
}

template <typename T> ValuePtr<T> Value<T>::log() {
    auto thisValue = this->shared_from_this();

    ValuePtr<T> result = Value<T>::create(std::log(_data));
    result->_children = {thisValue};
    result->_backward = [thisValue, result]() { thisValue->_gradient += (1 / thisValue->_data) * result->_gradient; };

    return result;
}

template <typename T> ValuePtr<T> Value<T>::pow(ValuePtr<T> exponent) {
    auto thisValue = this->shared_from_this();

    ValuePtr<T> result = Value<T>::create(std::pow(_data, exponent->_data));
    result->_children = {thisValue, exponent};
    result->_backward = [thisValue, exponent, result]() {
        thisValue->_gradient += (exponent->_data * std::pow(thisValue->_data, exponent->_data - 1)) * result->_gradient;
        exponent->_gradient +=
            (std::pow(thisValue->_data, exponent->_data) * std::log(thisValue->_data)) * result->_gradient;
    };

    return result;
}

template <typename T> std::vector<ValuePtr<T>> Value<T>::topologicalSort() {
    auto timer = utils::startTimer();

    std::vector<ValuePtr<T>> sorted;
    std::unordered_set<Value<T> *> visited;

    std::stack<Value<T> *> stack;
    stack.push(this);

    while (!stack.empty()) {
        auto cur = stack.top();
        if (visited.find(cur) == visited.end()) {
            bool hasUnvisitedChildren = false;
            for (auto v : cur->_children) {
                if (visited.find(v.get()) == visited.end()) {
                    stack.push(v.get());
                    hasUnvisitedChildren = true;
                }
            }

            if (!hasUnvisitedChildren) {
                stack.pop();
                sorted.push_back(cur->shared_from_this());
                visited.insert(cur);
            }
        } else {
            stack.pop();
        }
    }

    topoSortTime += utils::stopTimer(timer);

    return sorted;
}

template <typename T> void Value<T>::backward() {
    _gradient = 1;
    std::vector<ValuePtr<T>> sorted = topologicalSort();

    for (auto val = sorted.rbegin(); val != sorted.rend(); val++) {
        (*val)->_backward();
    }

    for (auto s : sorted) {
        s->_children = {};
        s->_backward = []() {};
    }
}

template <typename T> std::ostream &operator<<(std::ostream &os, const ValuePtr<T> &value) {
    os << "Value(data=" << value->_data << ")";
    return os;
}

} // namespace shkyera

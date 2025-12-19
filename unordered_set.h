#pragma once
#include <vector>
#include <functional>

template<typename T, typename Hash = std::hash<T>>
class unordered_set {
public:
    unordered_set() = default;
    void insert(const T& v) {}
    bool contains(const T& v) const { return false; }
};

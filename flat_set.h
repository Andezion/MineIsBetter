#pragma once
#include <vector>
#include <algorithm>

template<typename T>
class flat_set {
public:
    void insert(const T& v) { data_.push_back(v); std::sort(data_.begin(), data_.end()); }
    bool contains(const T& v) const { return std::binary_search(data_.begin(), data_.end(), v); }
private:
    std::vector<T> data_;
};

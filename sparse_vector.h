#pragma once
#include <unordered_map>

template<typename Index, typename T>
class sparse_vector {
public:
    T& operator[](Index i) { return data_[i]; }
    bool contains(Index i) const { return data_.count(i); }
private:
    std::unordered_map<Index, T> data_;
};

#pragma once
#include <vector>
#include <cstddef>

template<typename T>
class fenwick_tree {
public:
    explicit fenwick_tree(std::size_t n): n_(n), data_(n+1) {}
    void add(std::size_t i, T delta) {
        for (++i; i <= n_; i += i & -i) data_[i] += delta;
    }
    T sum(std::size_t i) const {
        T r{}; for (++i; i; i -= i & -i) r += data_[i]; return r;
    }
private:
    std::size_t n_;
    std::vector<T> data_;
};

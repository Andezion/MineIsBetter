#pragma once
#include <vector>
#include <cstddef>

class disjoint_set {
public:
    explicit disjoint_set(std::size_t n): parent_(n), rank_(n,0) {
        for (std::size_t i = 0; i < n; ++i) parent_[i] = i;
    }

    std::size_t find(std::size_t x) {
        if (parent_[x] != x) parent_[x] = find(parent_[x]);
        return parent_[x];
    }

    bool unite(std::size_t a, std::size_t b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) parent_[a] = b;
        else if (rank_[b] < rank_[a]) parent_[b] = a;
        else { parent_[b] = a; ++rank_[a]; }
        return true;
    }

    bool same(std::size_t a, std::size_t b) { return find(a) == find(b); }

private:
    std::vector<std::size_t> parent_;
    std::vector<int> rank_;
};

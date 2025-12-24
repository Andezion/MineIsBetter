#pragma once

#include <map>

template<typename K, typename V>
class b_plus_tree {
public:
    void insert(const K& k, const V& v) { data_[k] = v; }
    V* find(const K& k) {
        auto it = data_.find(k);
        if (it == data_.end()) return nullptr;
        return &it->second;
    }

private:
    std::map<K,V> data_;
};

#pragma once
#include <vector>
#include <utility>
#include <optional>
#include <functional>

template<typename K, typename V, typename Hash = std::hash<K>>
class unordered_map {
public:
    unordered_map() = default;
    std::optional<V> get(const K& key) const { return {}; }
    void insert(const K& key, const V& val) {  }
    bool contains(const K& key) const { return false; }
};

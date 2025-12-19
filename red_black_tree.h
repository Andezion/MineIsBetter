#pragma once

template<typename K, typename V>
class red_black_tree {
public:
    void insert(const K&, const V&) {}
    V* find(const K&) { return nullptr; }
};

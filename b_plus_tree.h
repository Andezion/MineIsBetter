#pragma once

template<typename K, typename V>
class b_plus_tree {
public:
    void insert(const K&, const V&) {}
    V* find(const K&) { return nullptr; }
};

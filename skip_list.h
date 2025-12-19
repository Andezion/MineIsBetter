#pragma once

template<typename K, typename V>
class skip_list {
public:
    void insert(const K&, const V&) {}
    V* find(const K&) { return nullptr; }
};

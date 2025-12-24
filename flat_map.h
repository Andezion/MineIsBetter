#pragma once
#include <vector>
#include <algorithm>

template<typename K, typename V>
class flat_map
{
public:
    void insert(const K& k, const V& v)
    {
        data_.emplace_back(k,v);
        std::sort(data_.begin(), data_.end(), [](auto &a, auto &b)
        {
            return a.first < b.first;
        });
    }

    V* find(const K& k)
    {
        auto it = std::lower_bound(data_.begin(), data_.end(), k, [](auto &a, const K& val)
        {
            return a.first < val;
        });
        if (it != data_.end() && it->first==k)
        {
            return &it->second;
        }
        return nullptr;
    }
private:
    std::vector<std::pair<K,V>> data_;
};

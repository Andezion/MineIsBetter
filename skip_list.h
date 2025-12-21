#pragma once

#include <vector>

template<typename K, typename V>
class skip_list 
{
public:
    void insert(const K& k, const V& v) 
    {
        for (auto& kv : data_) 
        {
            if (kv.first == k) 
            { 
                kv.second = v; 
                return; 
            }
        }
        data_.emplace_back(k, v);
    }

    V* find(const K& k) 
    {
        for (auto& kv : data_) 
        {
            if (kv.first == k) 
            {
                return &kv.second;
            }
        }
        return nullptr;
    }

private:
    std::vector<std::pair<K,V>> data_;
};

#pragma once

#include <unordered_map>
#include <vector>

template<typename Index, typename T>
class sparse_vector {
public:
    T& operator[](Index i) { return data_[i]; }

    const T* get(Index i) const
    {
        auto it = data_.find(i);
        if (it == data_.end()) return nullptr;
        return &it->second;
    }

    bool contains(Index i) const { return data_.count(i) != 0; }

    void erase(Index i) { data_.erase(i); }

    void clear() noexcept { data_.clear(); }

    size_t size() const noexcept { return data_.size(); }

    std::vector<Index> keys() const
    {
        std::vector<Index> out;
        out.reserve(data_.size());
        for (auto const& kv : data_) out.push_back(kv.first);
        return out;
    }

private:
    std::unordered_map<Index, T> data_;
};

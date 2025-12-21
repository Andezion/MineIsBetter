#pragma once
#include <vector>
#include <utility>
#include <optional>
#include <functional>

template<typename K, typename V, typename Hash = std::hash<K>>
class unordered_map 
{
public:
    unordered_map() = default;
    std::optional<V> get(const K& key) const 
    {
        if (buckets_.empty()) 
        {
            return {};
        }
        const auto& bucket = buckets_[Hash{}(key) % buckets_.size()];
        for (const auto& kv : bucket) if (kv.first == key) return kv.second;
        return {};
    }

    void insert(const K& key, const V& val) {
        if (buckets_.empty()) buckets_.resize(16);
        auto& bucket = buckets_[Hash{}(key) % buckets_.size()];
        for (auto& kv : bucket) {
            if (kv.first == key) { kv.second = val; return; }
        }
        bucket.emplace_back(key, val);
        ++size_;
        if (size_ > buckets_.size()) rehash(buckets_.size() * 2);
    }

    bool contains(const K& key) const {
        if (buckets_.empty()) return false;
        const auto& bucket = buckets_[Hash{}(key) % buckets_.size()];
        for (const auto& kv : bucket) if (kv.first == key) return true;
        return false;
    }

private:
    void rehash(std::size_t new_bucket_count) {
        std::vector<std::vector<std::pair<K,V>>> new_buckets(new_bucket_count);
        for (const auto& bucket : buckets_) {
            for (const auto& kv : bucket) {
                new_buckets[Hash{}(kv.first) % new_bucket_count].push_back(kv);
            }
        }
        buckets_.swap(new_buckets);
    }

    std::vector<std::vector<std::pair<K,V>>> buckets_;
    std::size_t size_ = 0;
};

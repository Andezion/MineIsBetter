#pragma once
#include <vector>
#include <functional>
#include <algorithm>

template<typename T, typename Hash = std::hash<T>>
class unordered_set 
{
public:
    unordered_set(): buckets_(16), size_(0) {}

    void insert(const T& v) 
    {
        if (contains(v)) 
        {
            return;
        }

        if (size_ + 1 > buckets_.size()) 
        {
            rehash(buckets_.size() * 2);
        }   
        auto& bucket = buckets_[Hash{}(v) % buckets_.size()];

        bucket.push_back(v);
        ++size_;
    }

    bool contains(const T& v) const 
    {
        if (buckets_.empty()) 
        {
            return false;
        }
        
        const auto& bucket = buckets_[Hash{}(v) % buckets_.size()];
        return std::find(bucket.begin(), bucket.end(), v) != bucket.end();
    }

private:
    void rehash(std::size_t new_bucket_count) 
    {
        std::vector<std::vector<T>> new_buckets(new_bucket_count);
        for (const auto& bucket : buckets_) 
        {
            for (const auto& val : bucket) 
            {
                new_buckets[Hash{}(val) % new_bucket_count].push_back(val);
            }
        }
        buckets_.swap(new_buckets);
    }

    std::vector<std::vector<T>> buckets_;
    std::size_t size_;
};

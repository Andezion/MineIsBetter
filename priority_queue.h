#pragma once
#include <vector>
#include <algorithm>

template<typename T>
class priority_queue
{
public:
    void push(const T& v)
    {
        data_.push_back(v);
        std::push_heap(data_.begin(), data_.end());
    }

    void pop()
    {
        std::pop_heap(data_.begin(), data_.end()); data_.pop_back();
    }

    const T& top() const
    {
        return data_.front();
    }

    [[nodiscard]] bool empty() const
    {
        return data_.empty();
    }
private:
    std::vector<T> data_;
};

#pragma once
#include <vector>
#include <cstddef>
#include <stdexcept>

template<typename T>
class ring_buffer
{
public:
    explicit ring_buffer(std::size_t capacity): buf_(capacity), head_(0), tail_(0), full_(false) {}
    void push(const T& v)
    {
        buf_[tail_] = v;
        tail_ = (tail_ + 1) % buf_.size();

        if (full_)
        {
            head_ = (head_ + 1) % buf_.size();
        }
        full_ = tail_ == head_;
    }

    T pop()
    {
        if (empty())
        {
            throw std::out_of_range("empty");
        }

        T v = buf_[head_];
        head_ = (head_ + 1) % buf_.size();
        full_ = false;
        return v;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return !full_ && head_==tail_;
    }
    [[nodiscard]] bool full() const noexcept
    {
        return full_;
    }
    [[nodiscard]] std::size_t capacity() const noexcept
    {
        return buf_.size();
    }
private:
    std::vector<T> buf_;
    std::size_t head_, tail_;
    bool full_;
};

#pragma once
#include <cstddef>
#include <utility>
#include <stdexcept>

template<typename T, std::size_t Capacity>
class static_vector
{
public:
    static_vector() : sz_(0) {}

    [[nodiscard]] std::size_t size() const noexcept
    {
        return sz_;
    }

    static constexpr std::size_t capacity() noexcept
    {
        return Capacity;
    }

    void push_back(const T& v)
    {
        if (sz_ >= Capacity)
        {
            throw std::out_of_range("static_vector overflow");
        }
        new(&data_[sz_]) T(v);
        ++sz_;
    }

    void pop_back()
    {
        if (sz_)
        {
            --sz_;
            get_at(sz_).~T();
        }
    }

    T& operator[](const std::size_t i)
    {
        return get_at(i);
    }
    const T& operator[](const std::size_t i) const
    {
        return get_at(i);
    }
private:
    alignas(T) unsigned char data_[sizeof(T)*Capacity]{};
    std::size_t sz_;
    T& get_at(const std::size_t i)
    {
        return *reinterpret_cast<T*>(data_ + sizeof(T)*i);
    }
    const T& get_at(const std::size_t i) const
    {
        return *reinterpret_cast<const T*>(data_ + sizeof(T)*i);
    }
};

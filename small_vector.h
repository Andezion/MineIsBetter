#pragma once
#include <vector>
#include <cstddef>
#include <utility>

template<typename T, std::size_t N = 8>
class small_vector
{
public:
    small_vector() noexcept = default;
    ~small_vector()
    {
        clear();
    }

    void push_back(const T& v)
    {
        data().push_back(v);
    }
    void push_back(T&& v)
    {
        data().push_back(std::move(v));
    }
    template<class... Args> void emplace_back(Args&&... args)
    {
        data().emplace_back(std::forward<Args>(args)...);
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return data().size();
    }
    [[nodiscard]] bool empty() const noexcept
    {
        return data().empty();
    }
    T& operator[](std::size_t i)
    {
        return data()[i];
    }
    const T& operator[](std::size_t i) const
    {
        return data()[i];
    }

    void clear() noexcept
    {
        data().clear();
    }

private:
    std::vector<T> storage_;
    std::vector<T>& data()
    {
        return storage_;
    }
    const std::vector<T>& data() const
    {
        return storage_;
    }
};

#pragma once

template<typename T>
class list
{
    T * storage;
    size_t size_of_list{};
public:
    list();
    explicit list(size_t size);
    list(const T & value, size_t size);
    list(const list<T>& other);
    list(list<T>&& other) noexcept;
    list(T* begin, T* end);
    list(std::initializer_list<T> init);

    void assign(size_t size, const T & value);
    T& back() const;
    T* begin() const;
    const T* cbegin() const noexcept;
    const T* cend() const noexcept;
    void clear();
    const T& crbegin() const noexcept;
    const T* crend() const noexcept;
    T emplace(size_t pos, const T & value);

    ~list();
};
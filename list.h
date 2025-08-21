#pragma once
#include <list>
#include <stdexcept>

template<typename T>
class list
{
    T * storage;
    size_t size_of_list{};
public:
    list();
    explicit list(size_t size);
    list(const T & value, size_t size);
    list(const list& other);
    list(list&& other) noexcept;
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

template<typename T>
list<T>::list()
{
    storage = nullptr;
    size_of_list = 0;
}

template<typename T>
list<T>::list(const size_t size)
{
    storage = new T[size];
    size_of_list = size;
}

template<typename T>
list<T>::list(const T &value, size_t size)
{
    storage = new T[size];
    size_of_list = size;

    for (size_t i = 0; i < size; i++)
    {
        storage[i] = value;
    }
}


template<typename T>
list<T>::list(const list<T> &other)
{
    storage = new T[other.size_of_list];
    size_of_list = other.size_of_list;

    for (size_t i = 0; i < size_of_list; i++)
    {
        storage[i] = other.storage[i];
    }
}

template<typename T>
list<T>::list(list &&other) noexcept
{
    storage = other.storage;
    size_of_list = other.size_of_list;

    delete[] other.storage;
    other.storage = nullptr;
    other.size_of_list = 0;
}

template<typename T>
list<T>::list(T *begin, T *end)
{
    storage = new T[end - begin];
    size_of_list = end - begin;
    for (size_t i = 0; i < size_of_list; i++)
    {
        storage[i] = begin[i];
    }
}

template<typename T>
list<T>::list(std::initializer_list<T> init)
{
    storage = new T[init.size()];
    size_of_list = init.size();
    for (size_t i = 0; i < size_of_list; i++)
    {
        storage[i] = init[i];
    }
}

template<typename T>
void list<T>::assign(const size_t size, const T &value)
{
    storage = new T[size];
    size_of_list = size;
    for (size_t i = 0; i < size; i++)
    {
        storage[i] = value;
    }
}

template<typename T>
T & list<T>::back() const
{
    if (size_of_list == 0)
    {
        throw std::out_of_range("Array is empty");
    }
    return *(storage + size_of_list - 1);
}

template<typename T>
T * list<T>::begin() const
{
    return storage;
}

template<typename T>
const T * list<T>::cbegin() const noexcept
{
    return storage;
}

template<typename T>
const T * list<T>::cend() const noexcept
{
    return storage + size_of_list;
}

template<typename T>
void list<T>::clear()
{
    delete[] storage;
    storage = nullptr;
    size_of_list = 0;
}

template<typename T>
const T & list<T>::crbegin() const noexcept
{
    return storage + size_of_list;
}

template<typename T>
const T * list<T>::crend() const noexcept
{
    return storage;
}

template<typename T>
T list<T>::emplace(size_t pos, const T &value)
{
    if (pos >= size_of_list)
    {
        throw std::out_of_range("Array is empty");
    }
    storage[pos] = value;
    return storage[pos];
}

template<typename T>
list<T>::~list()
{
    if (storage != nullptr)
    {
        delete[] storage;
        storage = nullptr;

        size_of_list = 0;
    }
}






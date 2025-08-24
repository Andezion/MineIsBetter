#pragma once

#include <stdexcept>

template<typename T>
class array
{
    T * storage;
    size_t size_of_array{};
public:
    array();
    explicit array(size_t size);
    array(const T & value, size_t size);

    ~array();

    T& at(size_t index) const;
    T& back() const;
    T* begin() const noexcept;
    const T* cbegin() const;
    const T* cend() const;
    const T* crbegin() const noexcept;
    const T* crend() const noexcept;
    T* data() const noexcept;
    bool empty() const noexcept;
    T* end() const noexcept;
    void fill(const T & value);
    T& front() const;
    constexpr size_t max_size() const noexcept;
    T& operator[](size_t index) const;
    const T* rbegin() const noexcept;
    const T* rend() const noexcept;
    constexpr size_t size() const noexcept;
    void swap(array & other) noexcept;
};

template<typename T>
array<T>::array()
{
    storage = nullptr;
    size_of_array = 0;
}

template<typename T>
array<T>::array(const size_t size)
{
    storage = new T[size];
    size_of_array = size;
}

template<typename T>
array<T>::array(const T & value, const size_t size)
{
    storage = new T[size];
    size_of_array = size;

    for (size_t i = 0; i < size_of_array; i++)
    {
        storage[i] = value;
    }
}

template<typename T>
array<T>::~array()
{
    if (storage != nullptr)
    {
        delete[] storage;
        storage = nullptr;

        size_of_array = 0;
    }
}

template<typename T>
T& array<T>::at(size_t index) const
{
    if(index >= size_of_array)
    {
        throw std::out_of_range("Index out of range");
    }

    return *(storage + index);
}

template<typename T>
T& array<T>::back() const
{
    if (size_of_array == 0)
    {
        throw std::out_of_range("Array is empty");
    }
    return *(storage + size_of_array - 1);
}

template<typename T>
T* array<T>::begin() const noexcept
{
    return storage;
}

template<typename T>
const T* array<T>::cbegin() const
{
    return storage;
}

template<typename T>
const T* array<T>::cend() const
{
    return storage + size_of_array;
}

template<typename T>
const T* array<T>::crbegin() const noexcept
{
    return storage + size_of_array;
}

template<typename T>
const T* array<T>::crend() const noexcept
{
    return storage;
}

template<typename T>
T* array<T>::data() const noexcept
{
    return storage;
}

template<typename T>
bool array<T>::empty() const noexcept
{
    if (size_of_array == 0)
    {
        return true;
    }
    return false;
}

template<typename T>
T *array<T>::end() const noexcept
{
    return storage + size_of_array;
}

template<typename T>
void array<T>::fill(const T &value)
{
    for (size_t i = 0; i < size_of_array; i++)
    {
        storage[i] = value;
    }
}

template<typename T>
T &array<T>::front() const
{
    if (size_of_array == 0)
    {
        throw std::out_of_range("Array is empty");
    }
    return *storage;
}

template<typename T>
constexpr size_t array<T>::max_size() const noexcept
{
    return size_of_array;
}

template<typename T>
T &array<T>::operator[](size_t index) const
{
    if (index >= size_of_array)
    {
        throw std::out_of_range("Index out of range");
    }
    return *(storage + index);
}

template<typename T>
const T *array<T>::rbegin() const noexcept
{
    return storage + size_of_array - 1;
}

template<typename T>
const T *array<T>::rend() const noexcept
{
    return storage;
}

template<typename T>
constexpr size_t array<T>::size() const noexcept
{
    return size_of_array;
}

template<typename T>
void array<T>::swap(array<T> &other) noexcept
{
    std::swap(storage, other.storage);
    std::swap(size_of_array, other.size_of_array);
}
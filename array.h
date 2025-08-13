#pragma once

#include <stdexcept>

template<typename T>
class array
{
    T * data;
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
    T& front() const;
};

template<typename T>
array<T>::array()
{
    data = nullptr;
    size_of_array = 0;
}

template<typename T>
array<T>::array(const size_t size)
{
    data = new T[size];
    size_of_array = size;
}

template<typename T>
array<T>::array(const T & value, const size_t size)
{
    data = new T[size];
    size_of_array = size;

    for (size_t i = 0; i < size_of_array; i++)
    {
        data[i] = value;
    }
}

template<typename T>
array<T>::~array()
{
    if (data != nullptr)
    {
        delete[] data;
        data = nullptr;

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

    return *(data + index);
}

template<typename T>
T& array<T>::back() const
{
    if (size_of_array == 0)
    {
        throw std::out_of_range("Array is empty");
    }
    return *(data + size_of_array - 1);
}

template<typename T>
T* array<T>::begin() const noexcept
{
    return data;
}

template<typename T>
const T* array<T>::cbegin() const
{
    return data;
}

template<typename T>
const T* array<T>::cend() const
{
    return data + size_of_array;
}

template<typename T>
T &array<T>::front() const
{
    if (size_of_array == 0)
    {
        throw std::out_of_range("Array is empty");
    }
    return *data;
}


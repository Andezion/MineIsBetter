#pragma once

#include <limits>
#include <stdexcept>
#include <utility>

template<typename T>
class vector
{
    T * data;
    size_t size_of_vector{};
    size_t capacity_of_vector{};

public:
    vector();
    explicit vector(size_t size);
    vector(size_t size, const T& value);

    ~vector();

    void push_back(T value);
    void pop_back();
    void emplace_back(T value);

    T& operator[](size_t index);
    const T & operator[](size_t index) const;
    T& at(size_t index);
    T& back();
    T& front();

    void resize(size_t new_size, const T& value = T());
    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t capacity() const;
    [[nodiscard]] static size_t max_size();

    [[nodiscard]] bool empty() const;
    void clear();

    void reserve(size_t new_capacity);

    T* begin();
    T* end();
};

template<typename T>
size_t vector<T>::size() const
{
    return size_of_vector;
}

template<typename T>
size_t vector<T>::capacity() const
{
    return capacity_of_vector;
}

template<typename T>
size_t vector<T>::max_size()
{
    return std::numeric_limits<size_t>::max();
}

template<typename T>
bool vector<T>::empty() const
{
    return size_of_vector == 0;
}

template<typename T>
void vector<T>::clear()
{
    if (size_of_vector != 0)
    {
        delete[] data;
        data = nullptr;
        size_of_vector = 0;
        capacity_of_vector = 0;
    }
}

template<typename T>
void vector<T>::reserve(const size_t new_capacity)
{
    if (new_capacity > capacity_of_vector)
    {
        T * new_data = new T[new_capacity];
        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
    }
    capacity_of_vector = new_capacity;
}

template<typename T>
T* vector<T>::begin()
{
    return data;
}

template<typename T>
T* vector<T>::end()
{
    return data + size_of_vector;
}

template<typename T>
void vector<T>::push_back(T value)
{
    if (size_of_vector >= capacity_of_vector)
    {
        if (capacity_of_vector == 0)
        {
            capacity_of_vector = 1;
        }
        else
        {
            capacity_of_vector = capacity_of_vector * 2;
        }

        T * new_data = new T[capacity_of_vector];
        for(int i = 0; i < size_of_vector; i++)
        {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
    }
    data[size_of_vector] = value;
    size_of_vector++;
}

template<typename T>
void vector<T>::pop_back()
{
    if (size_of_vector == 0)
    {
        return;
    }
    size_of_vector--;
}

template<typename T>
void vector<T>::emplace_back(T value)
{
    if (size_of_vector >= capacity_of_vector)
    {
        size_t new_capacity;
        if (capacity_of_vector == 0)
        {
            new_capacity = 1;
        }
        else
        {
            new_capacity = capacity_of_vector * 2;
        }

        T * new_data = new T[new_capacity];

        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
        capacity_of_vector = new_capacity;
    }
    data[size_of_vector] = std::move(value);
    size_of_vector++;
}

template<typename T>
T& vector<T>::operator[](size_t index)
{
    if(index >= size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }
    return *(data + index);
}

template<typename T>
const T& vector<T>::operator[](size_t index) const
{
    if(index >= size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }
    return *(data + index);
}

template<typename T>
T& vector<T>::at(size_t index)
{
    if(index >= size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }

    return *(data + index);
}

template<typename T>
T& vector<T>::back()
{
    return *(data + size_of_vector - 1);
}

template<typename T>
T& vector<T>::front()
{
    return *data;
}

template<typename T>
void vector<T>::resize(const size_t new_size, const T& value)
{
    if(new_size < size_of_vector)
    {
        size_of_vector = new_size;
    }
    else if(new_size > size_of_vector)
    {
        if(new_size > capacity_of_vector)
        {
            reserve(new_size);
        }

        for(size_t i = size_of_vector; i < new_size; i++)
        {
            data[i] = value;
        }
        size_of_vector = new_size;
    }
}

template<typename T>
vector<T>::vector()
{
    data = nullptr;

    size_of_vector = 0;
    capacity_of_vector = 0;
}

template<typename T>
vector<T>::vector(const size_t size)
{
    data = new T[size];
    size_of_vector = size;
    capacity_of_vector = size;
}

template<typename T>
vector<T>::vector(const size_t size, const T& value)
{
    data = new T[size];
    size_of_vector = size;
    capacity_of_vector = size;

    for (size_t i = 0; i < size_of_vector; i++)
    {
        data[i] = value;
    }
}

template<typename T>
vector<T>::~vector()
{
    if (data != nullptr)
    {
        delete[] data;

        data = nullptr;
        size_of_vector = 0;
        capacity_of_vector = 0;
    }
}


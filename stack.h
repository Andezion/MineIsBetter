#pragma once
#include <algorithm>

template<typename T>
class stack
{
    T * data;
    size_t size_of_stack{};
    size_t capacity_of_stack{};

public:
    stack();
    stack(const stack<T> & other);
    stack(stack<T> && other) noexcept;

    ~stack();

    void push(T value);
    void pop();

    void emplace(T value);
    bool empty() const;

    size_t size() const;

    void swap(stack<T> & other) noexcept;
    T & top();
    const T & top() const;

    stack<T> & operator=(const stack<T> & other);
    stack<T> & operator=(stack<T> && other) noexcept;
};

template<typename T>
stack<T>::stack()
{
    data = nullptr;

    size_of_stack = 0;
    capacity_of_stack = 0;
}

template<typename T>
stack<T>::stack(const stack<T> & other)
{
    size_of_stack = other.size_of_stack;
    capacity_of_stack = other.capacity_of_stack;

    data = new T[capacity_of_stack];
    for (size_t i = 0; i < size_of_stack; i++)
    {
        data[i] = other.data[i];
    }
}

template<typename T>
stack<T>::stack(stack<T> && other) noexcept
{
    data = other.data;
    size_of_stack = other.size_of_stack;
    capacity_of_stack = other.capacity_of_stack;

    other.data = nullptr;
    other.size_of_stack = 0;
    other.capacity_of_stack = 0;
}

template<typename T>
stack<T>::~stack()
{
    if(data != nullptr)
    {
        delete[] data;

        data = nullptr;

        size_of_stack = 0;
        capacity_of_stack = 0;
    }
}

template<typename T>
void stack<T>::push(T value)
{
    if (size_of_stack >= capacity_of_stack)
    {
        if (capacity_of_stack == 0)
        {
            capacity_of_stack = 1;
        }
        else
        {
            capacity_of_stack = capacity_of_stack * 2;
        }

        T * new_data = new T[capacity_of_stack];
        for(int i = 0; i < size_of_stack; i++)
        {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
    }
    data[size_of_stack] = value;
    size_of_stack++;
}

template<typename T>
void stack<T>::pop()
{
    if (size_of_stack == 0)
    {
        return;
    }

    size_of_stack--;
}

template<typename T>
void stack<T>::emplace(T value)
{
    if (size_of_stack >= capacity_of_stack)
    {
        size_t new_capacity;
        if (capacity_of_stack == 0)
        {
            new_capacity = 1;
        }
        else
        {
            new_capacity = capacity_of_stack * 2;
        }

        T * new_data = new T[new_capacity];

        for (size_t i = 0; i < size_of_stack; i++)
        {
            new_data[i] = std::move(data[i]);
        }

        delete[] data;
        data = new_data;
        capacity_of_stack = new_capacity;
    }
    data[size_of_stack] = std::move(value);
    size_of_stack++;
}

template<typename T>
bool stack<T>::empty() const
{
    if(size_of_stack == 0)
    {
        return true;
    }
    return false;
}

template<typename T>
size_t stack<T>::size() const
{
    return size_of_stack;
}

template<typename T>
void stack<T>::swap(stack<T> & other) noexcept
{
    std::swap(data, other.data);
    std::swap(size_of_stack, other.size_of_stack);
    std::swap(capacity_of_stack, other.capacity_of_stack);
}

template<typename T>
T & stack<T>::top()
{
    if (empty()) throw std::runtime_error("Stack is empty");
    return data[size_of_stack - 1];
}

template<typename T>
const T & stack<T>::top() const
{
    if(empty()) throw std::runtime_error("Stack is empty");
    return data[size_of_stack - 1];
}

template<typename T>
stack<T> & stack<T>::operator=(const stack<T> &other)
{
    if (this != &other)
    {
        stack<T> temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
stack<T> & stack<T>::operator=(stack<T> && other) noexcept
{
    data = other.data;
    size_of_stack = other.size_of_stack;
    capacity_of_stack = other.capacity_of_stack;

    other.data = nullptr;
    other.size_of_stack = 0;
    other.capacity_of_stack = 0;

    return *this;
}

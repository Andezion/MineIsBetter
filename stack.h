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
    ~stack();

    void push(T value);
    void pop();

    void emplace(T value);
    bool empty() const;

    size_t size() const;

    void swap(stack<T> &);
    T top() const;
};

template<typename T>
stack<T>::stack()
{
    data = nullptr;

    size_of_stack = 0;
    capacity_of_stack = 0;
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
            capacity_of_stack++;
        }

        T * new_data = new T[size_of_stack];
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

    if(size_of_stack > 0)
    {
        capacity_of_stack--;

        T * new_data = new T[capacity_of_stack];
        for(int i = 0; i < size_of_stack; i++)

    }
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
            new_capacity = capacity_of_stack + 1;
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

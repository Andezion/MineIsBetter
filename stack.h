#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <cstddef>
#include <algorithm>

template<typename T>
class stack
{
    T* data_;
    size_t size_;
    size_t capacity_;
    std::allocator<T> alloc_;

public:
    stack() noexcept;
    stack(const stack<T>& other);
    stack(stack<T>&& other) noexcept;
    ~stack() noexcept;

    stack<T>& operator=(const stack<T>& other);
    stack<T>& operator=(stack<T>&& other) noexcept;

    void push(const T& value);
    void push(T&& value);
    template<typename... Args>
    void emplace(Args&&... args);
    void pop();

    T& top();
    const T& top() const;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void swap(stack<T>& other) noexcept;

private:
    void grow();
    void clear_and_deallocate() noexcept;
};

template<typename T>
stack<T>::stack() noexcept : data_(nullptr), size_(0), capacity_(0) {}

template<typename T>
stack<T>::stack(const stack<T>& other) : data_(nullptr), size_(0), capacity_(0)
{
    if (other.size_ == 0) return;
    data_ = alloc_.allocate(other.capacity_);
    capacity_ = other.capacity_;
    size_t i = 0;
    try {
        for (; i < other.size_; ++i)
            alloc_.construct(data_ + i, other.data_[i]);
        size_ = other.size_;
    } catch (...) {
        for (size_t j = 0; j < i; ++j) alloc_.destroy(data_ + j);
        alloc_.deallocate(data_, capacity_);
        data_ = nullptr; capacity_ = 0; size_ = 0;
        throw;
    }
}

template<typename T>
stack<T>::stack(stack<T>&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<typename T>
stack<T>::~stack() noexcept
{
    clear_and_deallocate();
}

template<typename T>
stack<T>& stack<T>::operator=(const stack<T>& other)
{
    if (this == &other) return *this;
    stack<T> tmp(other);
    swap(tmp);
    return *this;
}

template<typename T>
stack<T>& stack<T>::operator=(stack<T>&& other) noexcept
{
    if (this == &other) return *this;
    clear_and_deallocate();
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
}

template<typename T>
void stack<T>::push(const T& value)
{
    emplace(value);
}

template<typename T>
void stack<T>::push(T&& value)
{
    emplace(std::move(value));
}

template<typename T>
template<typename... Args>
void stack<T>::emplace(Args&&... args)
{
    if (size_ == capacity_) grow();
    alloc_.construct(data_ + size_, std::forward<Args>(args)...);
    ++size_;
}

template<typename T>
void stack<T>::pop()
{
    if (size_ == 0) return;
    --size_;
    alloc_.destroy(data_ + size_);
}

template<typename T>
T& stack<T>::top()
{
    if (empty()) throw std::runtime_error("Stack is empty");
    return data_[size_ - 1];
}

template<typename T>
const T& stack<T>::top() const
{
    if (empty()) throw std::runtime_error("Stack is empty");
    return data_[size_ - 1];
}

template<typename T>
bool stack<T>::empty() const noexcept { return size_ == 0; }

template<typename T>
size_t stack<T>::size() const noexcept { return size_; }

template<typename T>
void stack<T>::swap(stack<T>& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename T>
void stack<T>::grow()
{
    const size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    T* new_data = alloc_.allocate(new_capacity);
    size_t i = 0;
    try {
        for (; i < size_; ++i)
            alloc_.construct(new_data + i, std::move_if_noexcept(data_[i]));
    } catch (...) {
        for (size_t j = 0; j < i; ++j) alloc_.destroy(new_data + j);
        alloc_.deallocate(new_data, new_capacity);
        throw;
    }
    for (size_t j = 0; j < size_; ++j) alloc_.destroy(data_ + j);
    if (data_) alloc_.deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T>
void stack<T>::clear_and_deallocate() noexcept
{
    if (!data_) return;
    for (size_t i = 0; i < size_; ++i) alloc_.destroy(data_ + i);
    alloc_.deallocate(data_, capacity_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}



#pragma once

template<typename T>
class unique_ptr
{
    T* ptr_ = nullptr;

public:
    unique_ptr() noexcept = default;
    explicit unique_ptr(T* p) noexcept : ptr_(p) {}
    ~unique_ptr()
    {
        delete ptr_;
    }

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;

    unique_ptr(unique_ptr&& o) noexcept : ptr_(o.ptr_)
    {
        o.ptr_ = nullptr;
    }

    unique_ptr& operator=(unique_ptr&& o) noexcept
    {
        if (this != &o)
        {
            reset(o.release());
        }
        return *this;
    }

    T* get() const noexcept
    {
        return ptr_;
    }

    T* release() noexcept
    {
        T* p = ptr_;
        ptr_ = nullptr;
        return p;
    }

    void reset(T* p = nullptr) noexcept
    {
        delete ptr_; ptr_ = p;
    }

    T& operator*() const noexcept
    {
        return *ptr_;
    }
    T* operator->() const noexcept
    {
        return ptr_;
    }
    explicit operator bool() const noexcept
    {
        return ptr_ != nullptr;
    }
};

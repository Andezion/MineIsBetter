#pragma once

#include <atomic>
#include <utility>

template<typename T>
class shared_ptr;

template<typename T>
class weak_ptr;

template<typename T>
struct control_block
{
    std::atomic_size_t strong;
    std::atomic_size_t weak;
    T* ptr;

    explicit control_block(T* p) : strong(1), weak(1), ptr(p) {}
    void destroy_object() { delete ptr; ptr = nullptr; }
    void delete_self() { delete this; }
};

template<typename T>
class shared_ptr
{
    T* ptr_ = nullptr;
    control_block<T>* ctrl_ = nullptr;

    template<typename U> friend class shared_ptr;
    template<typename U> friend class weak_ptr;

public:
    shared_ptr() noexcept = default;
    explicit shared_ptr(T* p) : ptr_(p), ctrl_(p ? new control_block<T>(p) : nullptr) {}

    ~shared_ptr() { release(); }

    shared_ptr(const shared_ptr& o) noexcept : ptr_(o.ptr_), ctrl_(o.ctrl_)
    {
        if (ctrl_) ctrl_->strong.fetch_add(1, std::memory_order_relaxed);
    }

    shared_ptr& operator=(const shared_ptr& o) noexcept
    {
        if (this != &o)
        {
            release();
            ptr_ = o.ptr_; ctrl_ = o.ctrl_;
            if (ctrl_) ctrl_->strong.fetch_add(1, std::memory_order_relaxed);
        }
        return *this;
    }

    shared_ptr(shared_ptr&& o) noexcept : ptr_(o.ptr_), ctrl_(o.ctrl_)
    {
        o.ptr_ = nullptr; o.ctrl_ = nullptr;
    }

    shared_ptr& operator=(shared_ptr&& o) noexcept
    {
        if (this != &o)
        {
            release();
            ptr_ = o.ptr_; ctrl_ = o.ctrl_;
            o.ptr_ = nullptr; o.ctrl_ = nullptr;
        }
        return *this;
    }

    T* get() const noexcept { return ptr_; }
    T& operator*() const noexcept { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

    long use_count() const noexcept { return ctrl_ ? static_cast<long>(ctrl_->strong.load(std::memory_order_relaxed)) : 0; }

    void reset() noexcept { release(); }
    void reset(T* p)
    {
        release();
        if (p) ctrl_ = new control_block<T>(p);
        ptr_ = p;
    }

    static shared_ptr make_from_control(control_block<T>* c) noexcept
    {
        shared_ptr s;
        if (!c) return s;
        s.ctrl_ = c;
        s.ptr_ = c->ptr;
        return s;
    }

private:
    void release() noexcept
    {
        if (!ctrl_) { ptr_ = nullptr; return; }

        size_t prev = ctrl_->strong.fetch_sub(1);
        if (prev == 1)
        {
            ctrl_->destroy_object();
            if (ctrl_->weak.fetch_sub(1) == 1)
            {
                ctrl_->delete_self();
            }
        }
        ptr_ = nullptr;
        ctrl_ = nullptr;
    }
};

template<typename T>
class weak_ptr
{
    control_block<T>* ctrl_ = nullptr;

public:
    weak_ptr() noexcept = default;
    weak_ptr(const shared_ptr<T>& sp) noexcept : ctrl_(sp.ctrl_) { if (ctrl_) ctrl_->weak.fetch_add(1, std::memory_order_relaxed); }
    weak_ptr(const weak_ptr& o) noexcept : ctrl_(o.ctrl_) { if (ctrl_) ctrl_->weak.fetch_add(1, std::memory_order_relaxed); }

    weak_ptr& operator=(const weak_ptr& o) noexcept
    {
        if (this != &o)
        {
            if (ctrl_ && ctrl_->weak.fetch_sub(1) == 1 && ctrl_->strong.load() == 0)
            {
                ctrl_->delete_self();
            }
            ctrl_ = o.ctrl_;
            if (ctrl_) ctrl_->weak.fetch_add(1, std::memory_order_relaxed);
        }
        return *this;
    }

    ~weak_ptr() noexcept
    {
        if (ctrl_ && ctrl_->weak.fetch_sub(1) == 1 && ctrl_->strong.load() == 0)
        {
            ctrl_->delete_self();
        }
    }

    bool expired() const noexcept { return !ctrl_ || ctrl_->strong.load(std::memory_order_relaxed) == 0; }
    long use_count() const noexcept { return ctrl_ ? static_cast<long>(ctrl_->strong.load(std::memory_order_relaxed)) : 0; }

    shared_ptr<T> lock() const noexcept
    {
        if (!ctrl_) return shared_ptr<T>();

        size_t s = ctrl_->strong.load();
        while (s != 0)
        {
            if (ctrl_->strong.compare_exchange_weak(s, s + 1))
            {
                return shared_ptr<T>::make_from_control(ctrl_);
            }
        }
        return shared_ptr<T>();
    }
};

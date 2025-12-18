#pragma once

#include "deque.h"
#include <utility>
#include <cstddef>

template<typename T, typename Container = deque<T>>
class queue
{
public:
    using value_type = T;
    using container_type = Container;
    using size_type = typename container_type::size_type;

protected:
    container_type c_;

public:
    queue() = default;
    explicit queue(const container_type& cont): c_(cont) {}
    explicit queue(container_type&& cont): c_(std::move(cont)) {}

    [[nodiscard]] bool empty() const noexcept 
    { 
        return c_.empty(); 
    }
    [[nodiscard]] size_type size() const noexcept 
    { 
        return c_.size(); 
    }
    value_type& front() 
    { 
        return c_.front(); 
    }
    const value_type& front() const 
    { 
        return c_.front(); 
    }
    value_type& back() 
    { 
        return c_.back(); 
    }
    const value_type& back() const 
    { 
        return c_.back(); 
    }

    void push(const value_type& v) 
    { 
        c_.push_back(v); 
    }
    void push(value_type&& v) 
    { 
        c_.push_back(std::move(v)); 
    }
    template<class... Args>
    void emplace(Args&&... args) 
    { 
        c_.emplace_back(std::forward<Args>(args)...); 
    }
    void pop() 
    { 
        c_.pop_front(); 
    }

    void swap(queue& other) noexcept(noexcept(std::declval<container_type&>().swap(other.c_))) 
    { 
        c_.swap(other.c_); 
    }

    template <class U, class C> friend bool operator== (const queue<U,C>& lhs, const queue<U,C>& rhs) 
    { 
        return lhs.c_ == rhs.c_; 
    }
    template <class U, class C> friend bool operator!= (const queue<U,C>& lhs, const queue<U,C>& rhs) 
    { 
        return lhs.c_ != rhs.c_; 
    }
    template <class U, class C> friend bool operator<  (const queue<U,C>& lhs, const queue<U,C>& rhs) 
    { 
        return lhs.c_ < rhs.c_; 
    }
    template <class U, class C> friend bool operator<= (const queue<U,C>& lhs, const queue<U,C>& rhs) 
    { 
        return lhs.c_ <= rhs.c_; 
    }
    template <class U, class C> friend bool operator>  (const queue<U,C>& lhs, const queue<U,C>& rhs) 
    { 
        return lhs.c_ > rhs.c_; 
    }
    template <class U, class C> friend bool operator>= (const queue<U,C>& lhs, const queue<U,C>& rhs) 
    { 
        return lhs.c_ >= rhs.c_;
    }
};

template <class T, class Container>
void swap(queue<T,Container>& x, queue<T,Container>& y) noexcept(noexcept(x.swap(y))) { x.swap(y); }

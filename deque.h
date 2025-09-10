#pragma once
#include <initializer_list>

template<typename T>
class deque
{
    T * data;
    size_t size;
    size_t capacity;
    size_t front;
    size_t back;
public:
    explicit deque (const allocator_type& alloc = allocator_type());
    explicit deque (size_type n);
    deque (size_type n, const value_type& val, const allocator_type& alloc = allocator_type());
    template <class InputIterator> deque (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());
    deque (const deque& x);
    deque (const deque& x, const allocator_type& alloc);
    deque (deque&& x);deque (deque&& x, const allocator_type& alloc);
    deque (initializer_list<value_type> il, const allocator_type& alloc = allocator_type());

    template <class InputIterator>  void assign (InputIterator first, InputIterator last);
    void assign (T n, const T& val);
    void assign (std::initializer_list<T> il);
    reference at (T n);
    const_reference at (T n) const;
    reference back();
    const_reference back() const;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    void clear() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    template <class... Args>  iterator emplace (const_iterator position, Args&&... args);
    template <class... Args>  void emplace_back (Args&&... args);
    template <class... Args>  void emplace_front (Args&&... args);
    bool empty() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    iterator erase (const_iterator position);
    iterator erase (const_iterator first, const_iterator last);
    reference front();
    const_reference front() const;
};

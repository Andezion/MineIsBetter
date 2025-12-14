#pragma once

#include <memory>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <cstddef>
#include <utility>
#include <limits>

template<typename T>
class deque
{
public:
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;

private:
    allocator_type alloc_;
    T* data_ = nullptr;
    size_type sz_ = 0;
    size_type cap_ = 0;
    size_type head_ = 0;

    [[nodiscard]] size_type idx(const size_type pos) const noexcept
    {
        return (head_ + pos) % cap_;
    }
    void destroy_range(const size_type from, const size_type to) noexcept
    {
        if (!data_)
        {
            return;
        }
        for (size_type i = from; i != to; i = (i + 1) % cap_)
        {
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + i);
        }
    }

public:
    class iterator
    {
    public:
        deque* d_ = nullptr;
        size_type pos_ = 0;

        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() = default;
        iterator(deque* d, const size_type p): d_(d), pos_(p) {}

        reference operator*() const
        {
            return (*d_)[pos_];
        }
        pointer operator->() const
        {
            return &(*d_)[pos_];
        }

        iterator& operator++()
        {
            ++pos_;
            return *this;
        }
        iterator operator++(int)
        {
            iterator t = *this;
            ++*this; return t;
        }
        iterator& operator--()
        {
            --pos_;
            return *this;
        }
        iterator operator--(int)
        {
            iterator t = *this;
            --*this; return t;
        }

        iterator& operator+=(const difference_type n)
        {
            pos_ += n;
            return *this;
        }
        iterator operator+(const difference_type n) const
        {
            return iterator(d_, pos_ + n);
        }
        iterator operator-(const difference_type n) const
        {
            return iterator(d_, pos_ - n);
        }
        difference_type operator-(const iterator& o) const
        {
            return static_cast<difference_type>(pos_) - static_cast<difference_type>(o.pos_);
        }

        bool operator==(const iterator& o) const
        {
            return d_ == o.d_ && pos_ == o.pos_;
        }
        bool operator!=(const iterator& o) const
        {
            return !(*this == o);
        }
        bool operator<(const iterator& o) const
        {
            return pos_ < o.pos_;
        }
    };

    using const_iterator = iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = reverse_iterator;

    explicit deque (const allocator_type& alloc = allocator_type());
    explicit deque (size_type n);
    deque (size_type n, const value_type& val, const allocator_type& alloc = allocator_type());
    template <class InputIterator> deque (InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type());
    deque (const deque& x);
    deque (const deque& x, const allocator_type& alloc);
    deque (deque&& x) noexcept ; deque (deque&& x, const allocator_type& alloc);
    deque (std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type());

    template <class InputIterator>  void assign (InputIterator first, InputIterator last);
    void assign (size_type n, const T& val);
    void assign (std::initializer_list<T> il);
    reference at (size_type n);
    const_reference at (size_type n) const;
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
    [[nodiscard]] bool empty() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    iterator erase (const_iterator position);
    iterator erase (const_iterator first, const_iterator last);
    reference front();
    const_reference front() const;
    allocator_type get_allocator() const;
    iterator insert (iterator position, const T& val);
    void insert (iterator position, size_type n, const T& val);
    template <class InputIterator> void insert (iterator position, InputIterator first, InputIterator last);

    static size_type max_size();
    reference operator[] (size_type n);
    const_reference operator[] (size_type n) const;
    deque& operator= (const deque& x);
    void pop_back();
    void pop_front();
    void push_back (const T& val);
    void push_front (const T& val);
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    void resize (size_type n, T val = T());
    void shrink_to_fit();
    [[nodiscard]] size_type size() const;
    void swap (deque& x) noexcept;

    template <class U>  friend bool operator== (const deque<U>& lhs, const deque<U>& rhs);
    template <class U>  friend bool operator!= (const deque<U>& lhs, const deque<U>& rhs);
    template <class U>  friend bool operator<  (const deque<U>& lhs, const deque<U>& rhs);
    template <class U>  friend bool operator<= (const deque<U>& lhs, const deque<U>& rhs);
    template <class U>  friend bool operator>  (const deque<U>& lhs, const deque<U>& rhs);
    template <class U>  friend bool operator>= (const deque<U>& lhs, const deque<U>& rhs);

    template <class U>  friend void swap (deque<U>& x, deque<U>& y);
};

template<typename T>
deque<T>::deque(const allocator_type& alloc)
    : alloc_(alloc), data_(nullptr) {}

template<typename T>
deque<T>::deque(const size_type n)
    : alloc_(allocator_type()), data_(nullptr)
{
    if (n > 0)
    {
        cap_ = n;
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, cap_);
        for (size_type i = 0; i < n; ++i)
        {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i);
        }
        sz_ = n;
    }
}

template<typename T>
deque<T>::deque(const size_type n, const value_type& val, const allocator_type& alloc)
    : alloc_(alloc), data_(nullptr)
{
    if (n > 0)
    {
        cap_ = n;
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, cap_);
        for (size_type i = 0; i < n; ++i)
        {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, val);
        }
        sz_ = n;
    }
}

template<typename T>
template <class InputIterator>
deque<T>::deque(InputIterator first, InputIterator last, const allocator_type& alloc)
    : alloc_(alloc), data_(nullptr)
{
    for (auto it = first; it != last; ++it)
    {
        push_back(*it);
    }
}

template<typename T>
deque<T>::deque(const deque& x)
    : alloc_(x.alloc_), data_(nullptr)
{
    if (x.sz_ > 0)
    {
        cap_ = x.sz_;
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, cap_);
        for (size_type i = 0; i < x.sz_; ++i)
        {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, x[i]);
        }
        sz_ = x.sz_;
    }
}

template<typename T>
deque<T>::deque(const deque& x, const allocator_type& alloc)
    : alloc_(alloc), data_(nullptr)
{
    if (x.sz_ > 0)
    {
        cap_ = x.sz_;
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, cap_);
        for (size_type i = 0; i < x.sz_; ++i)
        {
            std::allocator_traits<allocator_type>::construct(alloc_, data_ + i, x[i]);
        }
        sz_ = x.sz_;
    }
}

template<typename T>
deque<T>::deque(deque&& x)
    : alloc_(std::move(x.alloc_)), data_(x.data_), sz_(x.sz_), cap_(x.cap_), head_(x.head_)
{
    x.data_ = nullptr; x.sz_ = x.cap_ = x.head_ = 0;
}

template<typename T>
deque<T>::deque(deque&& x, const allocator_type& alloc)
    : alloc_(alloc), data_(nullptr)
{
    if (alloc_ == x.alloc_)
    {
        data_ = x.data_; sz_ = x.sz_; cap_ = x.cap_; head_ = x.head_;
        x.data_ = nullptr; x.sz_ = x.cap_ = x.head_ = 0;
    }
    else
    {
        for (size_type i = 0; i < x.sz_; ++i)
        {
            push_back(x[i]);
        }
    }
}

template<typename T>
deque<T>::deque(std::initializer_list<value_type> il, const allocator_type& alloc)
    : alloc_(alloc), data_(nullptr)
{
    for (const auto& v : il)
    {
        push_back(v);
    }
}

template<typename T>
template <class InputIterator>
void deque<T>::assign(InputIterator first, InputIterator last)
{
    clear();
    for (auto it = first; it != last; ++it)
    {
        push_back(*it);
    }
}

template<typename T>
void deque<T>::assign(const size_type n, const T& val)
{
    clear();
    for (size_type i = 0; i < n; ++i)
    {
        push_back(val);
    }
}

template<typename T>
void deque<T>::assign(std::initializer_list<T> il)
{
    assign(il.begin(), il.end());
}

template<typename T>
typename deque<T>::reference deque<T>::at(size_type n)
{
    if (n >= sz_)
    {
        throw std::out_of_range("deque::at");
    }
    return (*this)[n];
}

template<typename T>
typename deque<T>::const_reference deque<T>::at(size_type n) const
{
    if (n >= sz_)
    {
        throw std::out_of_range("deque::at");
    }
    return (*this)[n];
}

template<typename T>
typename deque<T>::reference deque<T>::back()
{
    return (*this)[sz_ - 1];
}

template<typename T>
typename deque<T>::const_reference deque<T>::back() const
{
    return (*this)[sz_ - 1];
}

template<typename T>
typename deque<T>::iterator deque<T>::begin() noexcept { return iterator(this, 0); }

template<typename T>
typename deque<T>::const_iterator deque<T>::begin() const noexcept { return const_iterator(const_cast<deque*>(this), 0); }

template<typename T>
typename deque<T>::const_iterator deque<T>::cbegin() const noexcept { return begin(); }

template<typename T>
typename deque<T>::const_iterator deque<T>::cend() const noexcept { return end(); }

template<typename T>
void deque<T>::clear() noexcept {
    if (data_) {
        for (size_type i = 0; i < sz_; ++i) {
            size_type p = idx(i);
            std::allocator_traits<allocator_type>::destroy(alloc_, data_ + p);
        }
    }
    sz_ = 0; head_ = 0;
}

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::crbegin() const noexcept { return const_reverse_iterator(end()); }

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::crend() const noexcept { return const_reverse_iterator(begin()); }

template<typename T>
template <class... Args>
typename deque<T>::iterator deque<T>::emplace(const_iterator position, Args&&... args) {
    size_type pos = static_cast<size_type>(position.pos_);
    insert(iterator(const_cast<deque*>(position.d_), pos), 1, value_type(std::forward<Args>(args)...));
    return iterator(this, pos);
}

template<typename T>
template <class... Args>
void deque<T>::emplace_back(Args&&... args) {
    push_back(value_type(std::forward<Args>(args)...));
}

template<typename T>
template <class... Args>
void deque<T>::emplace_front(Args&&... args) {
    push_front(value_type(std::forward<Args>(args)...));
}

template<typename T>
bool deque<T>::empty() const noexcept { return sz_ == 0; }

template<typename T>
typename deque<T>::iterator deque<T>::end() noexcept { return iterator(this, sz_); }

template<typename T>
typename deque<T>::const_iterator deque<T>::end() const noexcept { return const_iterator(const_cast<deque*>(this), sz_); }

template<typename T>
typename deque<T>::iterator deque<T>::erase(const_iterator position) {
    return erase(position, const_iterator(position.d_, position.pos_ + 1));
}

template<typename T>
typename deque<T>::iterator deque<T>::erase(const_iterator first, const_iterator last) {
    size_type f = static_cast<size_type>(first.pos_);
    size_type l = static_cast<size_type>(last.pos_);
    if (f >= l) return iterator(this, f);
    for (size_type i = l; i < sz_; ++i) {
        (*this)[i - (l - f)] = std::move((*this)[i]);
    }
    size_type removed = l - f;
    for (size_type i = 0; i < removed; ++i) pop_back();
    return iterator(this, f);
}

template<typename T>
typename deque<T>::reference deque<T>::front() { return (*this)[0]; }

template<typename T>
typename deque<T>::const_reference deque<T>::front() const { return (*this)[0]; }

template<typename T>
typename deque<T>::allocator_type deque<T>::get_allocator() const { return alloc_; }

template<typename T>
typename deque<T>::iterator deque<T>::insert(iterator position, const T& val) {
    size_type pos = static_cast<size_type>(position.pos_);
    insert(position, 1, val);
    return iterator(this, pos);
}

template<typename T>
void deque<T>::insert(iterator position, size_type n, const T& val) {
    size_type pos = static_cast<size_type>(position.pos_);
    if (n == 0) return;
    size_type need = sz_ + n;
    if (need > cap_) {
        size_type newcap = std::max<size_type>(cap_ ? cap_ * 2 : 1, need);
        T* newdata = std::allocator_traits<allocator_type>::allocate(alloc_, newcap);
        for (size_type i = 0; i < pos; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, newdata + i, std::move((*this)[i]));
        }
        for (size_type i = 0; i < n; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, newdata + pos + i, val);
        }
        for (size_type i = pos; i < sz_; ++i) {
            std::allocator_traits<allocator_type>::construct(alloc_, newdata + n + i, std::move((*this)[i]));
        }
        clear();
        if (data_) std::allocator_traits<allocator_type>::deallocate(alloc_, data_, cap_);
        data_ = newdata; cap_ = newcap; head_ = 0; sz_ = need;
    } else {
        for (size_type i = sz_; i > pos; --i) {
            (*this)[i] = std::move((*this)[i-1]);
        }
        for (size_type i = 0; i < n; ++i) {
            (*this)[pos + i] = val;
        }
        sz_ += n;
    }
}

template<typename T>
template <class InputIterator>
void deque<T>::insert(iterator position, InputIterator first, InputIterator last) {
    size_type pos = static_cast<size_type>(position.pos_);
    for (auto it = first; it != last; ++it) {
        insert(iterator(this, pos++), *it);
    }
}

template<typename T>
typename deque<T>::size_type deque<T>::max_size()
{
    return std::numeric_limits<size_type>::max() / sizeof(T);
}

template<typename T>
typename deque<T>::reference deque<T>::operator[](const size_type n)
{
    size_type p = idx(n);
    return data_[p];
}

template<typename T>
typename deque<T>::const_reference deque<T>::operator[](const size_type n) const
{
    size_type p = idx(n);
    return data_[p];
}

template<typename T>
deque<T>& deque<T>::operator=(const deque& x)
{
    if (this != &x)
    {
        clear();
        alloc_ = x.alloc_;
        for (size_type i = 0; i < x.sz_; ++i)
        {
            push_back(x[i]);
        }
    }
    return *this;
}

template<typename T>
void deque<T>::pop_back()
{
    if (sz_ == 0)
    {
        return;
    }
    size_type p = idx(sz_ - 1);
    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + p);
    --sz_;
}

template<typename T>
void deque<T>::pop_front()
{
    if (sz_ == 0)
    {
        return;
    }
    std::allocator_traits<allocator_type>::destroy(alloc_, data_ + head_);
    head_ = (head_ + 1) % cap_;
    --sz_;
}

template<typename T>
void deque<T>::push_back(const T& val)
{
    if (cap_ == 0)
    {
        cap_ = 1;
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, cap_);
    }
    else if (sz_ + 1 > cap_)
    {
        size_type newcap = cap_ * 2;
        T* newdata = std::allocator_traits<allocator_type>::allocate(alloc_, newcap);
        for (size_type i = 0; i < sz_; ++i)
        {
            std::allocator_traits<allocator_type>::construct(alloc_, newdata + i, std::move((*this)[i]));
        }
        clear();
        std::allocator_traits<allocator_type>::deallocate(alloc_, data_, cap_);
        data_ = newdata; cap_ = newcap; head_ = 0;
    }
    size_type p = idx(sz_);
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + p, val);
    ++sz_;
}

template<typename T>
void deque<T>::push_front(const T& val)
{
    if (cap_ == 0)
    {
        cap_ = 1;
        data_ = std::allocator_traits<allocator_type>::allocate(alloc_, cap_);
    }
    else if (sz_ + 1 > cap_)
    {
        size_type newcap = cap_ * 2;
        T* newdata = std::allocator_traits<allocator_type>::allocate(alloc_, newcap);
    
        for (size_type i = 0; i < sz_; ++i)
        {
            std::allocator_traits<allocator_type>::construct(alloc_, newdata + i + 1, std::move((*this)[i]));
        }

        clear();
        std::allocator_traits<allocator_type>::deallocate(alloc_, data_, cap_);
        data_ = newdata; cap_ = newcap; head_ = 0;
    }
    head_ = (head_ + cap_ - 1) % cap_;
    std::allocator_traits<allocator_type>::construct(alloc_, data_ + head_, val);
    ++sz_;
}

template<typename T>
typename deque<T>::reverse_iterator deque<T>::rbegin() { return reverse_iterator(end()); }

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::rbegin() const { return const_reverse_iterator(end()); }

template<typename T>
typename deque<T>::reverse_iterator deque<T>::rend() { return reverse_iterator(begin()); }

template<typename T>
typename deque<T>::const_reverse_iterator deque<T>::rend() const { return const_reverse_iterator(begin()); }

template<typename T>
void deque<T>::resize(const size_type n, T val)
{
    while (sz_ > n) pop_back();
    while (sz_ < n) push_back(val);
}

template<typename T>
void deque<T>::shrink_to_fit()
{
    if (sz_ == cap_)
    {
        return;
    }
    if (sz_ == 0)
    {
        if (data_)
        {
            std::allocator_traits<allocator_type>::deallocate(alloc_, data_, cap_);
        }
        data_ = nullptr;
        cap_ = 0;
        head_ = 0;
        return;
    }

    T* newdata = std::allocator_traits<allocator_type>::allocate(alloc_, sz_);
    for (size_type i = 0; i < sz_; ++i)
    {
        std::allocator_traits<allocator_type>::construct(alloc_, newdata + i, std::move((*this)[i]));
    }

    clear();
    std::allocator_traits<allocator_type>::deallocate(alloc_, data_, cap_);
    data_ = newdata; cap_ = sz_; head_ = 0;
}

template<typename T>
typename deque<T>::size_type deque<T>::size() const
{
    return sz_;
}

template<typename T>
void deque<T>::swap(deque& x) noexcept
{
    using std::swap;
    swap(alloc_, x.alloc_);
    swap(data_, x.data_);
    swap(sz_, x.sz_);
    swap(cap_, x.cap_);
    swap(head_, x.head_);
}

template <class T>
bool operator==(const deque<T>& lhs, const deque<T>& rhs)
{
    if (lhs.sz_ != rhs.sz_)
    {
        return false;
    }
    for (typename deque<T>::size_type i = 0; i < lhs.sz_; ++i)
    {
        if (lhs != nullptr && rhs != nullptr)
        {
            if (lhs[i] != rhs[i])
            {
                return false;
            }
        }
    }
    return true;
}

template <class T>
bool operator!=(const deque<T>& lhs, const deque<T>& rhs) { return !(lhs == rhs); }

template <class T>
bool operator<(const deque<T>& lhs, const deque<T>& rhs)
{
    typename deque<T>::size_type n = std::min(lhs.sz_, rhs.sz_);
    for (typename deque<T>::size_type i = 0; i < n; ++i)
    {
        if (lhs[i] < rhs[i])
        {
            return true;
        }
        if (rhs[i] < lhs[i])
        {
            return false;
        }
    }
    return lhs.sz_ < rhs.sz_;
}

template <class T>
bool operator<=(const deque<T>& lhs, const deque<T>& rhs)
{
    return !(rhs < lhs);
}

template <class T>
bool operator>(const deque<T>& lhs, const deque<T>& rhs)
{
    return rhs < lhs;
}

template <class T>
bool operator>=(const deque<T>& lhs, const deque<T>& rhs)
{
    return !(lhs < rhs);
}

template <class T>
void swap(deque<T>& x, deque<T>& y) noexcept
{ 
    x.swap(y); 
}


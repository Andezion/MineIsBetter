#pragma once

#include <memory>
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

template <typename T, typename Alloc = std::allocator<T>>
class vector {
public:
    using value_type             = T;
    using allocator_type         = Alloc;
    using traits_type            = std::allocator_traits<allocator_type>;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = typename traits_type::pointer;
    using const_pointer          = typename traits_type::const_pointer;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using difference_type        = typename traits_type::difference_type;
    using size_type              = typename traits_type::size_type;

    constexpr vector() noexcept(std::is_nothrow_default_constructible<allocator_type>::value);
    explicit vector(const allocator_type& alloc) noexcept;
    explicit vector(size_type n, const allocator_type& alloc = allocator_type());
    vector(size_type n, const value_type& value, const allocator_type& alloc = allocator_type());
    template <typename InputIt,
              typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
    vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type());
    vector(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type());
    vector(const vector& other);
    vector(const vector& other, const allocator_type& alloc);
    vector(vector&& other) noexcept(std::is_nothrow_move_constructible<allocator_type>::value);
    vector(vector&& other, const allocator_type& alloc);

    ~vector();

    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept(
        std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value ||
        std::is_nothrow_move_assignable<allocator_type>::value);
    vector& operator=(std::initializer_list<value_type> il);

    template <typename InputIt,
              typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
    void assign(InputIt first, InputIt last);
    void assign(size_type n, const value_type& val);
    void assign(std::initializer_list<value_type> il);

    reference       at(size_type pos);
    const_reference at(size_type pos) const;
    reference       operator[](size_type pos) noexcept;
    const_reference operator[](size_type pos) const noexcept;
    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;
    pointer         data() noexcept;
    const_pointer   data() const noexcept;

    iterator               begin() noexcept;
    const_iterator         begin() const noexcept;
    const_iterator         cbegin() const noexcept;
    iterator               end() noexcept;
    const_iterator         end() const noexcept;
    const_iterator         cend() const noexcept;
    reverse_iterator       rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator       rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    [[nodiscard]] bool        empty() const noexcept;
    size_type   size() const noexcept;
    size_type   max_size() const noexcept;
    void        reserve(size_type new_cap);
    size_type   capacity() const noexcept;
    void        shrink_to_fit();


    void clear() noexcept;

    iterator insert(const_iterator pos, const value_type& value);
    iterator insert(const_iterator pos, value_type&& value);
    iterator insert(const_iterator pos, size_type n, const value_type& value);

    template <typename InputIt,
              typename = std::enable_if_t<!std::is_integral<InputIt>::value>>
    iterator insert(const_iterator pos, InputIt first, InputIt last);

    iterator insert(const_iterator pos, std::initializer_list<value_type> il);

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    template <class... Args>
    void emplace_back(Args&&... args);

    void push_back(const value_type& value);
    void push_back(value_type&& value);

    void pop_back();

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void resize(size_type count);
    void resize(size_type count, const value_type& value);

    void swap(vector& other) noexcept(
        traits_type::propagate_on_container_swap::value ||
        std::is_nothrow_swappable<allocator_type>::value);

    allocator_type get_allocator() const noexcept;

    friend bool operator==(const vector& lhs, const vector& rhs)
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(const vector& lhs, const vector& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator<(const vector& lhs, const vector& rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    friend bool operator>(const vector& lhs, const vector& rhs)
    {
        return rhs < lhs;
    }
    friend bool operator<=(const vector& lhs, const vector& rhs)
    {
        return !(rhs < lhs);
    }
    friend bool operator>=(const vector& lhs, const vector& rhs)
    {
        return !(lhs < rhs);
    }

private:
    allocator_type alloc_;
    pointer        data_ = nullptr;
    size_type      size_ = 0;
    size_type      capacity_ = 0;

    pointer allocate(size_type n);
    void deallocate(pointer p, size_type n) noexcept;

    void destroy_elements(pointer first, pointer last) noexcept;
    void move_range(pointer dest, pointer first, pointer last) noexcept;
    void copy_range(pointer dest, const_pointer first, const_pointer last);
    void reallocate_grow(size_type new_cap);

    template <typename InputIt>
    void construct_from_range(InputIt first, InputIt last, std::true_type /*is_forward_or_better*/);

    template <typename InputIt>
    void construct_from_range(InputIt first, InputIt last, std::false_type /*is_input*/);
    void swap_alloc_and_data(vector& other) noexcept;
    void check_range(size_type pos) const;
};

template<typename T, typename Alloc>
vector<T, Alloc>::vector(const vector &other)
    : alloc_(traits_type::select_on_container_copy_construction(other.alloc_))
    , size_(other.size_)
    , capacity_(other.capacity_)
{
    if (capacity_ > 0)
    {
        data_ = alloc_.allocate(capacity_);
        std::uninitialized_copy(other.begin(), other.end(), data_);
    }
}

template<typename T, typename Alloc>
vector<T, Alloc>::~vector()
{
    if (data_ != nullptr)
    {
        for (size_type i = 0; i < size_; ++i)
        {
            traits_type::destroy(alloc_, data_ + i);
        }
        alloc_.deallocate(data_, capacity_);
    }
}

template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector &other)
{
    if (this != &other)
    {
        for (size_type i = 0; i < size_; ++i)
        {
            traits_type::destroy(alloc_, data_ + i);
        }

        if (data_ != nullptr)
        {
            alloc_.deallocate(data_, capacity_);
        }

        if (traits_type::propagate_on_container_copy_assignment::value)
        {
            alloc_ = other.alloc_;
        }

        size_ = other.size_;
        capacity_ = other.capacity_;

        if (capacity_ > 0)
        {
            data_ = alloc_.allocate(capacity_);
            std::uninitialized_copy(other.begin(), other.end(), data_);
        }
        else
        {
            data_ = nullptr;
        }
    }
    return *this;
}

template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector &&other) noexcept(
    traits_type::propagate_on_container_move_assignment::value ||
    std::is_nothrow_move_assignable<allocator_type>::value)
{
    if (this != &other)
    {
        for (size_type i = 0; i < size_; ++i)
        {
            traits_type::destroy(alloc_, data_ + i);
        }
        if (data_ != nullptr)
        {
            alloc_.deallocate(data_, capacity_);
        }

        if (traits_type::propagate_on_container_move_assignment::value)
        {
            alloc_ = std::move(other.alloc_);
        }

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<value_type> il)
{
    for (size_type i = 0; i < size_; ++i)
    {
        traits_type::destroy(alloc_, data_ + i);
    }

    size_type new_size = il.size();

    if (new_size > capacity_)
    {
        if (data_ != nullptr)
        {
            alloc_.deallocate(data_, capacity_);
        }
        capacity_ = new_size;
        data_ = alloc_.allocate(capacity_);
    }

    size_ = new_size;
    std::uninitialized_copy(il.begin(), il.end(), data_);

    return *this;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::at(size_type pos)
{
    check_range(pos);
    return data_[pos];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::at(size_type pos) const
{
    check_range(pos);
    return data_[pos];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::operator[](size_type pos) noexcept
{
    return data_[pos];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::operator[](size_type pos) const noexcept
{
    return data_[pos];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::front()
{
    return data_[0];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::front() const
{
    return data_[0];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reference vector<T, Alloc>::back()
{
    return data_[size_ - 1];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reference vector<T, Alloc>::back() const
{
    return data_[size_ - 1];
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::pointer vector<T, Alloc>::data() noexcept
{
    return data_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_pointer vector<T, Alloc>::data() const noexcept
{
    return data_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::begin() noexcept
{
    return data_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::begin() const noexcept
{
    return data_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cbegin() const noexcept
{
    return data_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::end() noexcept
{
    return data_ + size_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::end() const noexcept
{
    return data_ + size_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_iterator vector<T, Alloc>::cend() const noexcept
{
    return data_ + size_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::reverse_iterator vector<T, Alloc>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::const_reverse_iterator vector<T, Alloc>::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

template<typename T, typename Alloc>
bool vector<T, Alloc>::empty() const noexcept
{
    return size_ == 0;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::size() const noexcept
{
    return size_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::max_size() const noexcept
{
    return traits_type::max_size(alloc_);
}

template<typename T, typename Alloc>
void vector<T, Alloc>::reserve(size_type new_cap)
{
    if (new_cap <= capacity_)
    {
        return;
    }

    pointer new_data = alloc_.allocate(new_cap);

    for (size_type i = 0; i < size_; ++i)
    {
        traits_type::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
    }

    for (size_type i = 0; i < size_; ++i)
    {
        traits_type::destroy(alloc_, data_ + i);
    }

    if (data_ != nullptr)
    {
        alloc_.deallocate(data_, capacity_);
    }

    data_ = new_data;
    capacity_ = new_cap;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::size_type vector<T, Alloc>::capacity() const noexcept
{
    return capacity_;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::shrink_to_fit()
{
    if (capacity_ > size_)
    {
        if (size_ == 0)
        {
            if (data_ != nullptr)
            {
                alloc_.deallocate(data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            }
        }
        else
        {
            pointer new_data = alloc_.allocate(size_);

            for (size_type i = 0; i < size_; ++i)
            {
                traits_type::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
            }

            for (size_type i = 0; i < size_; ++i)
            {
                traits_type::destroy(alloc_, data_ + i);
            }

            alloc_.deallocate(data_, capacity_);

            data_ = new_data;
            capacity_ = size_;
        }
    }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::clear() noexcept
{
    for (size_type i = 0; i < size_; ++i)
    {
        traits_type::destroy(alloc_, data_ + i);
    }
    size_ = 0;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, const value_type &value)
{
    size_type index = pos - begin();

    if (size_ == capacity_)
    {
        size_type new_cap;
        if (capacity_ == 0)
        {
            new_cap = 1;
        }
        else
        {
            new_cap = capacity_ * 2;
        }
        reserve(new_cap);
    }

    for (size_type i = size_; i > index; --i)
    {
        if (i == size_)
        {
            traits_type::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i - 1]));
        }
        else
        {
            data_[i] = std::move_if_noexcept(data_[i - 1]);
        }
    }

    if (index < size_)
    {
        data_[index] = value;
    }
    else
    {
        traits_type::construct(alloc_, data_ + index, value);
    }
    ++size_;

    return begin() + index;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(const_iterator pos, value_type &&value)
{
    size_type index = pos - begin();
    if (size_ == capacity_)
    {
        size_type new_cap;
        if (capacity_ == 0)
        {
            new_cap = 1;
        }
        else
        {
            new_cap = capacity_ * 2;
        }
        reserve(new_cap);
    }

    for (size_type i = size_; i > index; --i)
    {
        if (i == size_)
        {
            traits_type::construct(alloc_, data_ + i, std::move_if_noexcept(data_[i - 1]));
        }
        else
        {
            data_[i] = std::move_if_noexcept(data_[i - 1]);
        }
    }

    if (index < size_)
    {
        data_[index] = std::move_if_noexcept(value);
    }
    else
    {
        traits_type::construct(alloc_, data_ + index, value);
    }
    ++size_;

    return begin() + index;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(const value_type& value)
{
    if (size_ >= capacity_)
    {
        size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        T * new_data = alloc_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i)
        {
            traits_type::construct(alloc_, new_data + i, data_[i]);
            traits_type::destroy(alloc_, data_ + i);
        }

        if (data_ != nullptr)
        {
            alloc_.deallocate(data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_cap;
    }

    traits_type::construct(alloc_, data_ + size_, value);
    ++size_;
}

template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(value_type&& value)
{
    if (size_ >= capacity_)
    {
        size_type new_cap;
        if (capacity_ == 0)
        {
            new_cap = 1;
        }
        else
        {
            new_cap = capacity_ * 2;
        }
        T * new_data = alloc_.allocate(new_cap);

        for (size_type i = 0; i < size_; ++i)
        {
            traits_type::construct(alloc_, new_data + i, std::move_if_noexcept(data_[i]));
            traits_type::destroy(alloc_, data_ + i);
        }

        if (data_ != nullptr)
        {
            alloc_.deallocate(data_, capacity_);
        }

        data_ = new_data;
        capacity_ = new_cap;
    }

    traits_type::construct(alloc_, data_ + size_, std::move(value));
    ++size_;
}


template<typename T, typename Alloc>
void vector<T, Alloc>::pop_back()
{
    if (size_ > 0)
    {
        --size_;
        std::allocator_traits<Alloc>::destroy(alloc_, data_ + size_);
    }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type count)
{
    if (count == size_)
    {
        return;
    }

    if (count < size_)
    {
        for (size_type i = count; i < size_; ++i)
        {
            (data_ + i)->~T();
        }
        size_ = count;
    }
    else
    {
        if (count > capacity_)
        {
            size_type new_capacity = std::max(count, capacity_ * 2);
            T* new_data = alloc_.allocate(new_capacity);

            for (size_type i = 0; i < size_; ++i)
            {
                new (new_data + i) T(std::move_if_noexcept(data_[i]));
                (data_ + i)->~T();
            }

            if (data_ != nullptr)
            {
                alloc_.deallocate(data_, capacity_);
            }

            data_ = new_data;
            capacity_ = new_capacity;
        }

        for (size_type i = size_; i < count; ++i)
        {
            new (data_ + i) T();
        }

        size_ = count;
    }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::resize(size_type count, const value_type &value)
{
    if (count < size_)
    {
        for (size_type i = count; i < size_; ++i)
        {
            (data_ + i)->~T();
        }
        size_ = count;
    }
    else if (count > size_)
    {
        if (count > capacity_)
        {
            size_type new_capacity = std::max(count, capacity_ * 2);
            T* new_data = alloc_.allocate(new_capacity);

            for (size_type i = 0; i < size_; ++i)
            {
                new (new_data + i) T(std::move_if_noexcept(data_[i]));
                (data_ + i)->~T();
            }

            if (data_ != nullptr)
            {
                alloc_.deallocate(data_, capacity_);
            }

            data_ = new_data;
            capacity_ = new_capacity;
        }

        for (size_type i = size_; i < count; ++i)
        {
            new (data_ + i) T(value);
        }

        size_ = count;
    }
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::allocator_type vector<T, Alloc>::get_allocator() const noexcept
{
    return alloc_;
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::pointer vector<T, Alloc>::allocate(size_type n)
{
    return traits_type::allocate(n);
}

template<typename T, typename Alloc>
void vector<T, Alloc>::deallocate(pointer p, size_type n) noexcept
{
    if (p != nullptr)
    {
        alloc_.deallocate(p, n);
    }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::destroy_elements(pointer first, pointer last) noexcept
{
    for (pointer p = first; p != last; ++p)
    {
        traits_type::destroy(p, p);
    }
}

template<typename T, typename Alloc>
void vector<T, Alloc>::check_range(size_type pos) const
{
    if (pos >= size_)
    {
        throw std::out_of_range("vector::check_range: pos >= size()");
    }
}

template <typename T, typename Alloc>
void swap(vector<T,Alloc>& a, vector<T,Alloc>& b) noexcept(noexcept(a.swap(b)))
{
    a.swap(b);
}
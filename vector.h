#pragma once

#include <memory>
#include <iterator>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cstddef>
#include <cassert>


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

    bool        empty() const noexcept;
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

    friend bool operator==(const vector& lhs, const vector& rhs) {
        if (lhs.size() != rhs.size()) return false;
        return std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    friend bool operator!=(const vector& lhs, const vector& rhs) { return !(lhs == rhs); }

    friend bool operator<(const vector& lhs, const vector& rhs) {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    friend bool operator>(const vector& lhs, const vector& rhs) { return rhs < lhs; }
    friend bool operator<=(const vector& lhs, const vector& rhs) { return !(rhs < lhs); }
    friend bool operator>=(const vector& lhs, const vector& rhs) { return !(lhs < rhs); }

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

template <typename T, typename Alloc>
void swap(vector<T,Alloc>& a, vector<T,Alloc>& b) noexcept(noexcept(a.swap(b))) {
    a.swap(b);
}


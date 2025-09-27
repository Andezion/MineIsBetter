#pragma once
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

template <class T, std::size_t N>
struct array {
    using value_type             = T;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    constexpr array() = default;
    constexpr array(const array&) = default;
    constexpr array(array&&) = default;
    constexpr ~array() = default;
    constexpr array& operator=(const array&) = default;
    constexpr array& operator=(array&&) = default;

    constexpr reference at(size_type pos);
    constexpr const_reference at(size_type pos) const;

    constexpr reference operator[](size_type pos);
    constexpr const_reference operator[](size_type pos) const;

    constexpr reference front();
    constexpr const_reference front() const;

    constexpr reference back();
    constexpr const_reference back() const;

    constexpr pointer data() noexcept;
    constexpr const_pointer data() const noexcept;

    constexpr iterator begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator cbegin() const noexcept;

    constexpr iterator end() noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cend() const noexcept;

    constexpr reverse_iterator rbegin() noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;

    constexpr reverse_iterator rend() noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    [[nodiscard]] constexpr bool empty() const noexcept;
    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;

    void fill(const T& value);
    void swap(array& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())));

    T elems[N ? N : 1];
};

template<class T, std::size_t N>
constexpr typename array<T, N>::reference array<T, N>::at(size_type pos)
{
    if (pos >= size())
    {
        throw std::out_of_range("array::at");
    }
    return elems[pos];
}

template <std::size_t I, class T, std::size_t N>
constexpr T& get(array<T, N>& arr) noexcept;

template <std::size_t I, class T, std::size_t N>
constexpr T&& get(array<T, N>&& arr) noexcept;

template <std::size_t I, class T, std::size_t N>
constexpr const T& get(const array<T, N>& arr) noexcept;

template <std::size_t I, class T, std::size_t N>
constexpr const T&& get(const array<T, N>&& arr) noexcept;

namespace std {
    template <class T, std::size_t N>
    struct tuple_size<array<T, N>> : std::integral_constant<std::size_t, N> {};

    template <std::size_t I, class T, std::size_t N>
    struct tuple_element<I, array<T, N>> {
        using type = T;
    };
}


template <class T, std::size_t N>
constexpr bool operator==(const array<T, N>& lhs, const array<T, N>& rhs);

template <class T, std::size_t N>
constexpr bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs);

template <class T, std::size_t N>
constexpr bool operator<(const array<T, N>& lhs, const array<T, N>& rhs);

template <class T, std::size_t N>
constexpr bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs);

template <class T, std::size_t N>
constexpr bool operator>(const array<T, N>& lhs, const array<T, N>& rhs);

template <class T, std::size_t N>
constexpr bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs);

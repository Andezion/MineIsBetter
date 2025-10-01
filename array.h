#pragma once
#include <cstddef>
#include <iterator>
#include <limits>
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

    [[nodiscard]] constexpr bool empty() noexcept;
    [[nodiscard]] constexpr size_type size() noexcept;
    [[nodiscard]] constexpr size_type max_size() noexcept;

    void fill(const T& value);
    void swap(array& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())));

    T elems[N ? N : 1];
};

template<class T, std::size_t N>
void array<T, N>::fill(const T &value)
{
    std::fill(elems, elems + N, value);
}

template<class T, std::size_t N>
void array<T, N>::swap(array &other) noexcept(noexcept(std::swap(std::declval<T &>(), std::declval<T &>())))
{
    using std::swap;
    swap(other.elems, elems + N);
}

template<class T, std::size_t N>
constexpr typename array<T, N>::reference array<T, N>::at(size_type pos)
{
    if (pos >= size())
    {
        throw std::out_of_range("array::at");
    }
    return elems[pos];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reference array<T, N>::at(size_type pos) const
{
    if (pos >= size())
    {
        throw std::out_of_range("array::at");
    }
    return elems[pos];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::reference array<T, N>::operator[](size_type pos)
{
    if (pos >= size())
    {
        throw std::out_of_range("array::at");
    }
    return elems[pos];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reference array<T, N>::operator[](size_type pos) const
{
    if (pos >= size())
    {
        throw std::out_of_range("array::at");
    }
    return elems[pos];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::reference array<T, N>::front()
{
    if (empty())
    {
        throw std::out_of_range("array::front");
    }
    return elems[0];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reference array<T, N>::front() const
{
    if (empty())
    {
        throw std::out_of_range("array::front");
    }
    return elems[0];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::reference array<T, N>::back()
{
    if (empty())
    {
        throw std::out_of_range("array::back");
    }
    return elems[size() - 1];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reference array<T, N>::back() const
{
    if (empty())
    {
        throw std::out_of_range("array::back");
    }
    return elems[size() - 1];
}

template<class T, std::size_t N>
constexpr typename array<T, N>::pointer array<T, N>::data() noexcept
{
    return elems;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_pointer array<T, N>::data() const noexcept
{
    return elems;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::iterator array<T, N>::begin() noexcept
{
    return elems + 0;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_iterator array<T, N>::begin() const noexcept
{
    return elems + 0;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_iterator array<T, N>::cbegin() const noexcept
{
    return elems + 0;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::iterator array<T, N>::end() noexcept
{
    return elems + size();
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_iterator array<T, N>::end() const noexcept
{
    return elems + size();
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_iterator array<T, N>::cend() const noexcept
{
    return elems + size();
}

template<class T, std::size_t N>
constexpr typename array<T, N>::reverse_iterator array<T, N>::rbegin() noexcept
{
    return elems + size();
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reverse_iterator array<T, N>::rbegin() const noexcept
{
    return elems + size();
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reverse_iterator array<T, N>::crbegin() const noexcept
{
    return elems + size();
}

template<class T, std::size_t N>
constexpr typename array<T, N>::reverse_iterator array<T, N>::rend() noexcept
{
    return elems + 0;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reverse_iterator array<T, N>::rend() const noexcept
{
    return elems + 0;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::const_reverse_iterator array<T, N>::crend() const noexcept
{
    return elems + 0;
}

template<class T, std::size_t N>
constexpr bool array<T, N>::empty() noexcept
{
    return size() == 0;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::size_type array<T, N>::size() noexcept
{
    return N;
}

template<class T, std::size_t N>
constexpr typename array<T, N>::size_type array<T, N>::max_size() noexcept
{
    return std::numeric_limits<size_type>::max();
}

template <std::size_t I, class T, std::size_t N>
constexpr T& get(array<T, N>& arr);

template<std::size_t I, class T, std::size_t N>
constexpr T & get(array<T, N> &arr)
{
    if (I >= N)
    {
        throw std::out_of_range("array::get");
    }
    return arr[I];
}

template <std::size_t I, class T, std::size_t N>
constexpr T&& get(array<T, N>&& arr);

template<std::size_t I, class T, std::size_t N>
constexpr T && get(array<T, N> &&arr)
{
    if (I >= N)
    {
        throw std::out_of_range("array::get");
    }
    return arr[I];
}

template <std::size_t I, class T, std::size_t N>
constexpr const T& get(const array<T, N>& arr);

template<std::size_t I, class T, std::size_t N>
constexpr const T & get(const array<T, N> &arr)
{
    if (I >= N)
    {
        throw std::out_of_range("array::get");
    }
    return arr[I];
}

template <std::size_t I, class T, std::size_t N>
constexpr const T&& get(const array<T, N>&& arr);

template<std::size_t I, class T, std::size_t N>
constexpr const T && get(const array<T, N> &&arr)
{
    if (I >= N)
    {
        throw std::out_of_range("array::get");
    }
    return arr[I];
}

namespace std {

    template <class T, std::size_t N>
    struct tuple_size<array<T, N>> : integral_constant<std::size_t, N> {};

    template <std::size_t I, class T, std::size_t N>
    struct tuple_element<I, array<T, N>> {
        static_assert(I < N, "Index out of bounds in tuple_element<std::array>");
        using type = T;
    };
}

template <class T, std::size_t N>
constexpr bool operator==(const array<T, N>& lhs, const array<T, N>& rhs);

template<class T, std::size_t N>
constexpr bool operator==(const array<T, N> &lhs, const array<T, N> &rhs)
{
    for (std::size_t i = 0; i < lhs.size(); i++)
    {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

template <class T, std::size_t N>
constexpr bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs);

template<class T, std::size_t N>
constexpr bool operator!=(const array<T, N> &lhs, const array<T, N> &rhs)
{
    for (std::size_t i = 0; i < N; i++)
    {
        if (rhs[i] == lhs[i]) return false;
    }
    return true;
}

template <class T, std::size_t N>
constexpr bool operator<(const array<T, N>& lhs, const array<T, N>& rhs);

template<class T, std::size_t N>
constexpr bool operator<(const array<T, N> &lhs, const array<T, N> &rhs)
{
    for (std::size_t i = 0; i < N; i++)
    {
        if (lhs[i] < rhs[i]) return true;
        if (rhs[i] < lhs[i]) return false;
    }
    return false;
}

template <class T, std::size_t N>
constexpr bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs);

template <class T, std::size_t N>
constexpr bool operator>(const array<T, N>& lhs, const array<T, N>& rhs);

template<class T, std::size_t N>
constexpr bool operator<=(const array<T, N> &lhs, const array<T, N> &rhs)
{
    for (std::size_t i = 0; i < N; i++)
    {
        if (lhs[i] <= rhs[i]) return true;
        if (rhs[i] < lhs[i]) return false;
    }
    return false;
}

template<class T, std::size_t N>
constexpr bool operator>(const array<T, N> &lhs, const array<T, N> &rhs)
{
    for (std::size_t i = 0; i < N; i++)
    {
        if (lhs[i] > rhs[i]) return true;
        if (rhs[i] > lhs[i]) return false;
    }
    return false;
}

template <class T, std::size_t N>
constexpr bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs);

template<class T, std::size_t N>
constexpr bool operator>=(const array<T, N> &lhs, const array<T, N> &rhs)
{
    for (std::size_t i = 0; i < N; i++)
    {
        if (lhs[i] >= rhs[i]) return true;
        if (rhs[i] > lhs[i]) return false;
    }
    return false;
}

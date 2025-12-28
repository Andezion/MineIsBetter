#pragma once

#include <iterator>
#include <functional>

namespace mib {

template<typename T>
void iter_swap(T a, T b)
{
    auto tmp = *a;
    *a = *b;
    *b = tmp;
}

template<typename RandomIt, typename Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp)
{
    for (auto it = first; it != last; ++it)
    {
        auto key = *it;
        auto j = it;

        while (j != first && comp(key, *std::prev(j)))
        {
            *j = *std::prev(j);
            --j;
        }
        *j = key;
    }
}

template<typename RandomIt, typename Compare>
void quick_sort(RandomIt first, RandomIt last, Compare comp) {
    using Diff = typename std::iterator_traits<RandomIt>::difference_type;
    Diff len = std::distance(first, last);
    if (len <= 1) return;
    if (len < 16) { insertion_sort(first, last, comp); return; }

    auto mid = first + len/2;

    if (comp(*mid, *first)) std::iter_swap(first, mid);
    if (comp(*(last-1), *first)) std::iter_swap(first, last-1);
    if (comp(*(last-1), *mid)) std::iter_swap(mid, last-1);

    auto pivot = *mid;
    RandomIt i = first; RandomIt j = last-1;
    while (true) {
        while (comp(*i, pivot)) ++i;
        while (comp(pivot, *j)) --j;
        if (i >= j) break;
        std::iter_swap(i, j);
        ++i; --j;
    }
    quick_sort(first, i, comp);
    quick_sort(i, last, comp);
}

template<typename RandomIt>
void sort(RandomIt first, RandomIt last) {
    sort(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

template<typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp) {
    quick_sort(first, last, comp);
}

template<typename ForwardIt, typename T, typename Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    using Diff = typename std::iterator_traits<ForwardIt>::difference_type;
    Diff len = std::distance(first, last);
    while (len > 0) {
        Diff half = len >> 1;
        ForwardIt mid = first;
        std::advance(mid, half);
        if (comp(*mid, value)) {
            first = ++mid;
            len -= half + 1;
        } else {
            len = half;
        }
    }
    return first;
}

template<typename ForwardIt, typename T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
    return lower_bound(first, last, value, std::less<typename std::iterator_traits<ForwardIt>::value_type>());
}

template<typename ForwardIt, typename T, typename Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    using Diff = typename std::iterator_traits<ForwardIt>::difference_type;
    Diff len = std::distance(first, last);
    while (len > 0) {
        Diff half = len >> 1;
        ForwardIt mid = first;
        std::advance(mid, half);
        if (!comp(value, *mid)) {
            first = ++mid;
            len -= half + 1;
        } else {
            len = half;
        }
    }
    return first;
}

template<typename ForwardIt, typename T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value) {
    return upper_bound(first, last, value, std::less<typename std::iterator_traits<ForwardIt>::value_type>());
}

template<typename ForwardIt, typename T, typename Compare>
bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    auto it = lower_bound(first, last, value, comp);
    return (it != last) && !comp(value, *it);
}

template<typename ForwardIt, typename T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value) {
    return binary_search(first, last, value, std::less<typename std::iterator_traits<ForwardIt>::value_type>());
}

template<typename RandomIt, typename Compare>
void push_heap(RandomIt first, RandomIt last, Compare comp)
{
    if (first == last)
    {
        return;
    }

    auto child = last - 1;
    while (child != first)
    {
        auto parent = first + (std::distance(first, child) - 1) / 2;
        if (comp(*parent, *child))
        {
            std::iter_swap(parent, child);
            child = parent;
        }
        else
        {
            break;
        }
    }
}

template<typename RandomIt>
void push_heap(RandomIt first, RandomIt last)
{
    push_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

template<typename RandomIt, typename Compare>
void pop_heap(RandomIt first, RandomIt last, Compare comp)
{
    if (last - first <= 1)
    {
        return;
    }

    std::iter_swap(first, last - 1);

    auto end = last - 1;
    auto parent = first;

    while (true)
    {
        auto left = first + (std::distance(first, parent) * 2 + 1);
        if (left >= end)
        {
            break;
        }

        auto right = left + 1;
        auto candidate = left;
        if (right < end && comp(*candidate, *right))
        {
            candidate = right;
        }

        if (comp(*parent, *candidate))
        {
            std::iter_swap(parent, candidate); parent = candidate;
        }
        else
        {
            break;
        }
    }
}

template<typename RandomIt>
void pop_heap(RandomIt first, RandomIt last)
{
    pop_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

template<typename RandomIt, typename Compare>
void make_heap(RandomIt first, RandomIt last, Compare comp)
{
    auto n = std::distance(first, last);
    if (n < 2)
    {
        return;
    }

    for (auto start = first + (n - 2) / 2; ; --start)
    {
        auto parent = start;
        while (true)
        {
            auto left = first + (std::distance(first, parent) * 2 + 1);
            if (left >= last)
            {
                break;
            }

            auto right = left + 1;
            auto candidate = left;

            if (right < last && comp(*candidate, *right))
            {
                candidate = right;
            }

            if (comp(*parent, *candidate))
            {
                std::iter_swap(parent, candidate);
                parent = candidate;
            }
            else
            {
                break;
            }
        }
        if (start == first)
        {
            break;
        }
    }
}

template<typename RandomIt>
void make_heap(RandomIt first, RandomIt last)
{
    make_heap(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
}

template<typename RandomIt>
void sort_heap(RandomIt first, RandomIt last)
{
    while (last - first > 1)
    {
        pop_heap(first, last);
        --last;
    }
}

}

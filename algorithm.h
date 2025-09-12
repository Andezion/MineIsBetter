#pragma once

// Min/max
template <class T> const T& min (const T& a, const T& b)
{
    if (b < a)
    {
        return b;
    }
    return a;
}
template <class T> const T& max (const T& a, const T& b)
{
    if (b > a)
    {
        return b;
    }
    return a;
}
template <class T>
std::pair <const T&,const T&> minmax (const T& a, const T& b)
{
    if (b < a)
    {
        return std::make_pair(b, a);
    }
    return std::make_pair(a, b);
}

template <class ForwardIterator> ForwardIterator min_element (ForwardIterator first, ForwardIterator last)
{
    if (first == last)
    {
        return last;
    }

    ForwardIterator smallest = first;

    while (++first != last)
    {
        if (*first < *smallest)
        {
            smallest = first;
        }
    }

    return smallest;
}

template <class ForwardIterator, class Compare> ForwardIterator min_element (ForwardIterator first, ForwardIterator last, Compare comp)
{
    if (first == last)
    {
        return last;
    }
    ForwardIterator smallest = first;
    while (++first != last)
    {
        if (comp(*first, *smallest))
        {
            smallest = first;
        }
    }
    return smallest;
}
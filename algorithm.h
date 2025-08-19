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

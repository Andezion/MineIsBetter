#pragma once
#include <vector>
#include <functional>

template<typename T>
class segment_tree 
{
public:
    segment_tree() = default;
    ~segment_tree() = default;

    explicit segment_tree(const std::vector<T>& a)
    {
        int n_ = 1;
        while (n_ < a.size())
        {
            n_ <<= 1;
        }

        tree_.assign(2 * n_, T{});

        for (size_t i = 0; i < a.size(); ++i)
        {
            tree_[n_ + i] = a[i];
        }
        for (size_t i = n_ - 1; i >= 1; --i)
        {
            tree_[i] = tree_[i << 1] + tree_[i << 1 | 1];
            if (i == 1) 
            {
                break;
            }
        }
    }
};

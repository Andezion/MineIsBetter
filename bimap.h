#pragma once
#include <utility>
#include <map>

template<typename L, typename R>
class bimap
{
public:
    void insert(const L& l, const R& r)
    {
        l2r_[l]=r;
        r2l_[r]=l;
    }

    bool contains_left(const L& l) const
    {
        return l2r_.count(l);
    }
    bool contains_right(const R& r) const
    {
        return r2l_.count(r);
    }
    const R* find_left(const L& l) const
    {
        auto it = l2r_.find(l);
        if(it == l2r_.end())
        {
            return nullptr;
        }
        return &it->second;
    }

    const L* find_right(const R& r) const
    {
        auto it = r2l_.find(r);
        if(it == r2l_.end())
        {
            return nullptr;
        }
        return &it->second;
    }
private:
    std::map<L,R> l2r_;
    std::map<R,L> r2l_;
};

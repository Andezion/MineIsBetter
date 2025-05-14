#include <iostream>

#include "vector.h"

int main()
{
    vector<int> v;
    for(int i = 0; i < 100; i++)
    {
        v.push_back(i);
    }

    for(const int i : v)
    {
        std::cout << i << std::endl;
    }

    return 0;
}

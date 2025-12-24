#include <iostream>
#include "avl_tree.h"
#include "red_black_tree.h"
#include "b_plus_tree.h"

int main()
{
    avl_tree<int, std::string> a;
    a.insert(2, "two");
    a.insert(1, "one");
    a.insert(3, "three");
    if (auto p = a.find(2)) std::cout << "AVL 2: " << *p << "\n";

    red_black_tree<int, std::string> r;
    r.insert(5, "five");
    r.insert(3, "three");
    r.insert(7, "seven");
    if (auto q = r.find(3)) std::cout << "RB 3: " << *q << "\n";

    b_plus_tree<int, std::string> b;
    b.insert(10, "ten");
    b.insert(20, "twenty");
    if (auto s = b.find(20)) std::cout << "B+ 20: " << *s << "\n";

    return 0;
}

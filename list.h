#pragma once

#include <stdexcept>
#include <utility>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include <initializer_list>

template<typename T>
class list
{
    struct Node
    {
        T value;
        Node* prev;
        Node* next;
        explicit Node(const T& v, Node* p=nullptr, Node* n=nullptr)
            : value(v), prev(p), next(n) {}
    };
    Node* head;
    Node* tail;
    size_t size_of_list;
public:
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = std::ptrdiff_t;
    struct iterator;
    struct const_iterator;

    struct const_iterator
    {
        const Node* current;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        explicit const_iterator(const Node* p = nullptr) : current(p) {}

        const T& operator*() const noexcept { return current->value; }
        const T* operator->() const noexcept { return &current->value; }
        const_iterator& operator++() noexcept { if (current) current = current->next; return *this; }
        const_iterator& operator--() noexcept { if (current) current = current->prev; else current = nullptr; return *this; }
        bool operator!=(const const_iterator& other) const noexcept { return current != other.current; }
        bool operator==(const const_iterator& other) const noexcept { return current == other.current; }

        const_iterator(const iterator& it) noexcept; // defined after iterator
    };

    struct iterator
    {
        Node* current;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator(Node* p = nullptr) : current(p) {}

        T& operator*() noexcept { return current->value; }
        T* operator->() noexcept { return &current->value; }
        iterator& operator++() noexcept { if (current) current = current->next; return *this; }
        iterator& operator--() noexcept { if (current) current = current->prev; else current = nullptr; return *this; }
        bool operator!=(const iterator& other) const noexcept { return current != other.current; }
        bool operator==(const iterator& other) const noexcept { return current == other.current; }
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    list();
    explicit list(size_t size);
    list(const list& other);
    list(list&& other) noexcept;
    list(std::initializer_list<T> init);

    list& operator=(const list& other);
    list& operator=(list&& other) noexcept;

    void assign(const list& other);
    void assign(list&& other) noexcept;
    void assign(std::initializer_list<T> init);
    void assign(size_t size, const T& value);
    void assign(T* first, T* last);

    T& back();
    const T& back() const;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;
    void resize(size_t n, const T& val = T());
    void clear() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    void emplace_back(const T& value);
    void emplace_front(const T& value);
    template<class... Args>
    void emplace_back(Args&&... args);
    template<class... Args>
    void emplace_front(Args&&... args);
    bool empty() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    T& front();
    const T& front() const;

    iterator insert (const_iterator position, const T& val);
    iterator insert (const_iterator position, T&& val);
    template<class... Args>
    iterator emplace(const_iterator position, Args&&... args);

    void unique();

    void merge (list& x);
    void merge (list&& x);
    template <class Compare> void merge (list& x, Compare comp);
    template <class Compare> void merge (list&& x, Compare comp);

    void splice (const_iterator position, list& x);
    void splice (const_iterator position, list&& x);
    void splice (const_iterator position, list& x, const_iterator i);
    void splice (const_iterator position, list&& x, const_iterator i);
    void splice (const_iterator position, list& x, const_iterator first, const_iterator last);
    void splice (const_iterator position, list&& x, const_iterator first, const_iterator last);

    void sort();

    void pop_back();
    void pop_front();

    void push_back (const T& val);
    void push_back (T&& val);
    void push_front (const T& val);
    void push_front (T&& val);

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;

    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;

    void swap (list& x) noexcept;
    ~list() noexcept;
};

template<typename T>
list<T>::const_iterator::const_iterator(const iterator& it) noexcept : current(it.current) {}


template<typename T>
list<T>::list()
{
    this->head = nullptr;
    this->tail = nullptr;
    this->size_of_list = 0;
}

template<typename T>
list<T>::list(size_t size)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->size_of_list = 0;
    
    this->assign(size, T());
}

template<typename T>
list<T>::list(const list &other)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->size_of_list = 0;

    assign(other);
}

template<typename T>
list<T>::list(list &&other) noexcept
{
    this->head = nullptr;
    this->tail = nullptr;

    this->size_of_list = 0;

    this->head = other.head;
    this->tail = other.tail;
    this->size_of_list = other.size_of_list;

    other.head = nullptr;
    other.tail = nullptr;

    other.size_of_list = 0;
}

template<typename T>
list<T>::list(std::initializer_list<T> init)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->size_of_list = 0;

    assign(init);
}

template<typename T>
list<T> & list<T>::operator=(const list &other)
{
    if (this == &other) 
    {
        return *this;
    }

    clear();

    for (Node* cur = other.head; cur; cur = cur->next)
    {
        push_back(cur->value);
    }
    return *this;
}

template<typename T>
list<T> & list<T>::operator=(list &&other) noexcept
{
    if (this == &other) return *this;

    clear();

    head = other.head;
    tail = other.tail;

    size_of_list = other.size_of_list;
    other.head = other.tail = nullptr;

    other.size_of_list = 0;
    return *this;
}

template<typename T>
void list<T>::assign(const list& other)
{
    if (this == &other)
    {
        return;
    }

    clear();
    for (Node* cur = other.head; cur; cur = cur->next)
    {
        push_back(cur->value);
    }
}

template<typename T>
void list<T>::assign(list &&other) noexcept
{
    if (this != &other)
    {
        clear();

        head = other.head;
        tail = other.tail;

        size_of_list = other.size_of_list;
        other.head = other.tail = nullptr;

        other.size_of_list = 0;
    }
    else
    {
        
    }
}

template<typename T>
void list<T>::assign(std::initializer_list<T> init)
{
    clear();
    for (const T& v : init) 
    {
        push_back(v);
    }
}

template<typename T>
void list<T>::assign(size_t size, const T &value)
{
    clear();
    for (size_t i = 0; i < size; ++i) 
    {
        push_back(value);
    }
}

template<typename T>
void list<T>::assign(T *first, T *last)
{
    clear();
    for (T* p = first; p != last; ++p) 
    {
        push_back(*p);
    }
}

template<typename T>
T & list<T>::back()
{
    if (size_of_list == 0)
    {
        throw std::out_of_range("list::back");
    }
    return tail->value;
}

template<typename T>
const T &list<T>::back() const
{
    if (size_of_list == 0)
    {
        throw std::out_of_range("list::back");
    }
    return tail->value;
}

template<typename T>
typename list<T>::iterator list<T>::begin() noexcept
{
    return iterator(head);
}

template<typename T>
typename list<T>::const_iterator list<T>::begin() const noexcept
{
    return const_iterator(head);
}

template<typename T>
typename list<T>::const_iterator list<T>::cbegin() const noexcept
{
    return const_iterator(head);
}

template<typename T>
typename list<T>::const_iterator list<T>::cend() const noexcept
{
    return const_iterator(nullptr);
}

template<typename T>
typename list<T>::size_type list<T>::size() const noexcept { return size_of_list; }

template<typename T>
typename list<T>::size_type list<T>::max_size() const noexcept { return std::numeric_limits<size_t>::max(); }

template<typename T>
void list<T>::resize(size_t n, const T& val)
{
    while (size_of_list > n) pop_back();
    while (size_of_list < n) push_back(val);
}

template<typename T>
list<T>::~list() noexcept
{
    Node* cur = head;
    while (cur)
    {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = tail = nullptr;
    size_of_list = 0;
}

template<typename T>
void list<T>::clear() noexcept
{
    Node* cur = head;
    while (cur)
    {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = tail = nullptr;
    size_of_list = 0;
}

template<typename T>
bool list<T>::empty() const noexcept 
{ 
    return size_of_list == 0; 
}

template<typename T>
void list<T>::push_back(const T& val)
{
    Node* n = new Node(val, tail, nullptr);

    if (!head) 
    {
        head = n;
    }
    if (tail) 
    {
        tail->next = n;
    }

    tail = n;
    ++size_of_list;
}

template<typename T>
void list<T>::push_back(T&& val)
{
    push_back(std::move(val));
}

template<typename T>
void list<T>::push_front(const T& val)
{
    Node* n = new Node(val, nullptr, head);

    if (!tail) 
    {
        tail = n;
    }
    if (head) 
    {
        head->prev = n;
    }

    head = n;
    ++size_of_list;
}

template<typename T>
void list<T>::push_front(T&& val)
{
    push_front(std::move(val));
}

template<typename T>
void list<T>::pop_back()
{
    if (!tail) 
    {
        return;
    }

    Node* p = tail;
    tail = tail->prev;

    if (tail) 
    {
        tail->next = nullptr; 
    }
    else 
    {
        head = nullptr;
    }

    delete p;
    --size_of_list;
}

template<typename T>
void list<T>::pop_front()
{
    if (!head) return;
    Node* p = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete p;
    --size_of_list;
}

template<typename T>
typename list<T>::iterator list<T>::end() noexcept { return iterator(nullptr); }

template<typename T>
typename list<T>::const_iterator list<T>::end() const noexcept { return const_iterator(nullptr); }

template<typename T>
T& list<T>::front() { if (!head) throw std::out_of_range("list::front"); return head->value; }

template<typename T>
const T& list<T>::front() const 
{ 
    if (!head) 
    {
        throw std::out_of_range("list::front");
    }
    return head->value; 
}

template<typename T>
typename list<T>::iterator list<T>::erase(const_iterator pos)
{
    if (!pos.current) 
    {
        return end();
    }

    Node* n = const_cast<Node*>(pos.current);
    Node* next = n->next;

    if (n->prev) 
    {
        n->prev->next = n->next; 
    }
    else head = n->next;


    if (n->next) 
    {
        n->next->prev = n->prev; 
    }
    else 
    {
        tail = n->prev;
    }

    delete n;
    --size_of_list;
    return iterator(next);
}

template<typename T>
typename list<T>::iterator list<T>::erase(const_iterator first, const_iterator last)
{
    auto it = first;
    while (it != last)
    {
        it = erase(it);
    }
    return iterator(last.current);
}

template<typename T>
typename list<T>::iterator list<T>::insert(const_iterator position, const T& val)
{
    if (!position.current)
    {
        push_back(val);
        return iterator(tail);
    }

    Node* cur = const_cast<Node*>(position.current);
    Node* n = new Node(val, cur->prev, cur);

    if (cur->prev) {
        cur->prev->next = n;
    } else {
        head = n;
    }

    cur->prev = n;
    ++size_of_list;
    return iterator(n);
}

template<typename T>
typename list<T>::iterator list<T>::insert(const_iterator position, T&& val)
{
    if (!position.current)
    {
        push_back(std::move(val));
        return iterator(tail);
    }

    Node* cur = const_cast<Node*>(position.current);
    Node* n = new Node(std::move(val), cur->prev, cur);

    if (cur->prev) {
        cur->prev->next = n;
    } else {
        head = n;
    }

    cur->prev = n;
    ++size_of_list;
    return iterator(n);
}

template<typename T>
template<class... Args>
typename list<T>::iterator list<T>::emplace(const_iterator position, Args&&... args)
{
    T tmp(std::forward<Args>(args)...);
    if (!position.current)
    {
        push_back(std::move(tmp));
        return iterator(tail);
    }

    Node* cur = const_cast<Node*>(position.current);
    Node* n = new Node(std::move(tmp), cur->prev, cur);

    if (cur->prev) {
        cur->prev->next = n;
    } else {
        head = n;
    }

    cur->prev = n;
    ++size_of_list;
    return iterator(n);
}

template<typename T>
void list<T>::emplace_back(const T& value) 
{ 
    push_back(value); 
}

template<typename T>
template<class... Args>
void list<T>::emplace_back(Args&&... args)
{
    T tmp(std::forward<Args>(args)...);
    Node* n = new Node(std::move(tmp), tail, nullptr);
    if (!head) head = n;
    if (tail) tail->next = n;
    tail = n;
    ++size_of_list;
}

template<typename T>
void list<T>::emplace_front(const T& value) 
{ 
    push_front(value); 
}

template<typename T>
template<class... Args>
void list<T>::emplace_front(Args&&... args)
{
    T tmp(std::forward<Args>(args)...);
    Node* n = new Node(std::move(tmp), nullptr, head);
    if (!tail) tail = n;
    if (head) head->prev = n;
    head = n;
    ++size_of_list;
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::crbegin() const noexcept 
{ 
    return const_reverse_iterator(end()); 
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::crend() const noexcept 
{ 
    return const_reverse_iterator(begin()); 
}

template<typename T>
typename list<T>::reverse_iterator list<T>::rbegin() noexcept 
{ 
    return reverse_iterator(end()); 
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::rbegin() const noexcept 
{ 
    return const_reverse_iterator(end()); 
}

template<typename T>
typename list<T>::reverse_iterator list<T>::rend() noexcept 
{ 
    return reverse_iterator(begin()); 
}

template<typename T>
typename list<T>::const_reverse_iterator list<T>::rend() const noexcept 
{ 
    return const_reverse_iterator(begin()); 
}

template<typename T>
void list<T>::swap(list& x) noexcept
{
    std::swap(head, x.head);
    std::swap(tail, x.tail);
    std::swap(size_of_list, x.size_of_list);
}

template<typename T>
void list<T>::unique()
{
    if (!head) return;
    Node* cur = head;
    while (cur && cur->next)
    {
        if (cur->value == cur->next->value)
        {
            Node* dup = cur->next;
            cur->next = dup->next;
            if (dup->next) dup->next->prev = cur; else tail = cur;
            delete dup;
            --size_of_list;
        }
        else
        {
            cur = cur->next;
        }
    }
}

template<typename T>
void list<T>::merge(list& x)
{
    if (this == &x) 
    {
        return;
    }
    
    Node dummy(T());
    Node* last = &dummy;
    Node* a = head;
    Node* b = x.head;

    while (a && b)
    {
        if (b->value < a->value)
        {
            Node* nb = b->next;

            b->prev = last == &dummy ? nullptr : last;

            last->next = b;

            b->next = nullptr; 

            last = b;
            b = nb;
        }
        else
        {
            Node* na = a->next;

            a->prev = last == &dummy ? nullptr : last;

            last->next = a;
            last = a;
            a = na;
        }
    }
    
    Node* rem = a ? a : b;
    while (rem)
    {
        Node* n = rem->next;
        rem->prev = last == &dummy ? nullptr : last;

        last->next = rem;
        last = rem;
        rem = n;
    }
    
    head = dummy.next;
    if (head) 
    {
        head->prev = nullptr;
    }

    tail = last == &dummy ? nullptr : last;
    
    size_of_list = 0;
    for (Node* p = head; p; p = p->next) 
    {
        ++size_of_list;
    }
    
    x.head = x.tail = nullptr;
    x.size_of_list = 0;
}

template<typename T>
void list<T>::merge(list&& x)
{
    merge(x);
}

template<typename T>
template<class Compare>
void list<T>::merge(list& x, Compare comp)
{
    if (this == &x) 
    {
        return;
    }

    Node dummy(T());
    Node* last = &dummy;

    Node* a = head;
    Node* b = x.head;

    while (a && b)
    {
        if (comp(b->value, a->value))
        {
            Node* nb = b->next;
            b->prev = last == &dummy ? nullptr : last;

            last->next = b;
            last = b;
            b = nb;
        }
        else
        {
            Node* na = a->next;
            a->prev = last == &dummy ? nullptr : last;

            last->next = a;
            last = a;
            a = na;
        }
    }

    Node* rem = a ? a : b;
    while (rem)
    {
        Node* n = rem->next;
        rem->prev = last == &dummy ? nullptr : last;

        last->next = rem;
        last = rem;
        rem = n;
    }

    head = dummy.next;

    if (head) 
    {
        head->prev = nullptr;
    }

    tail = last == &dummy ? nullptr : last;
    size_of_list = 0;

    for (Node* p = head; p; p = p->next) 
    {
        ++size_of_list;
    }

    x.head = x.tail = nullptr;
    x.size_of_list = 0;
}

template<typename T>
template<class Compare>
void list<T>::merge(list&& x, Compare comp)
{
    merge(x, comp);
}


template<typename T>
void list<T>::splice(const_iterator position, list& x)
{
    if (x.empty()) 
    {
        return;
    }

    if (this == &x) 
    {
        return;
    }

    Node* first = x.head;
    Node* lastn = x.tail;
    
    x.head = x.tail = nullptr;
    size_t moved = x.size_of_list;

    x.size_of_list = 0;
    
    if (!position.current)
    {
        if (!tail)
        {
            head = first; tail = lastn;
        }
        else
        {
            tail->next = first;
            first->prev = tail;
            tail = lastn;
        }
    }
    else
    {
        Node* pos = const_cast<Node*>(position.current);
        Node* prev = pos->prev;

        if (prev)
        {
            prev->next = first;
            first->prev = prev;
        }
        else
        {
            head = first;
            first->prev = nullptr;
        }
        lastn->next = pos;
        pos->prev = lastn;
    }
    size_of_list += moved;
}

template<typename T>
void list<T>::splice(const_iterator position, list&& x)
{
    splice(position, x);
}

template<typename T>
void list<T>::splice(const_iterator position, list& x, const_iterator i)
{
    if (!i.current) 
    {
        return;
    }

    Node* node = const_cast<Node*>(i.current);

    if (this == &x) 
{
        if (position.current == node || position.current == node->next) 
        {
            return;
        }
        
        if (node->prev) 
        {
            node->prev->next = node->next; 
        } 
        else 
        {
            head = node->next;
        }
        if (node->next) 
        {
            node->next->prev = node->prev; 
        }
        else 
        {
            tail = node->prev;
        }
        
    } 
    else 
    {
        
        if (node->prev) 
        {
            node->prev->next = node->next;
        } 
        else 
        {
            x.head = node->next;
        }
        if (node->next) 
        {
            node->next->prev = node->prev;
        } 
        else 
        {
            x.tail = node->prev;
        }
        --x.size_of_list;
    }

    if (!position.current)
    {
        node->prev = tail;
        node->next = nullptr;
        if (tail) 
        {
            tail->next = node;
        } 
        else 
        {
            head = node;
        }
        tail = node;
    }
    else
    {
        Node* pos = const_cast<Node*>(position.current);
        Node* prev = pos->prev;

        node->prev = prev;
        node->next = pos;
        pos->prev = node;

        if (prev) 
        {
            prev->next = node;
        } 
        else 
        {
            head = node;
        }
    }

    if (this != &x) 
    {
        ++size_of_list;
    }
}

template<typename T>
void list<T>::splice(const_iterator position, list&& x, const_iterator i)
{
    splice(position, x, i);
}

template<typename T>
void list<T>::splice(const_iterator position, list& x, const_iterator first, const_iterator last)
{
    if (this == &x) 
    {
        return;
    }
    if (first == last) 
    {
        return;
    }

    Node* f = const_cast<Node*>(first.current);
    Node* l = const_cast<Node*>(last.current);
    
    Node* before = f->prev;
    Node* after = l ? l : x.tail->next;
    
    Node* cur = f;
    size_t cnt = 0;
    while (cur != after)
    {
        Node* nx = cur->next;
        ++cnt;
        cur = nx;
    }

    if (before) 
    {
        before->next = after; 
    }
    else 
    {
        x.head = after;
    }
    if (after) 
    {
        after->prev = before; 
    }
    else 
    {
        x.tail = before;
    }

    x.size_of_list -= cnt;
    
    if (!position.current)
    {
        if (!tail)
        {
            head = f;
            
            Node* rn = f; 
            while (rn->next && rn->next != after) 
            {
                rn = rn->next;
            }
            tail = rn;
            tail->next = nullptr;
        }
        else
        {
            tail->next = f;
            f->prev = tail;

            Node* rn = f;

            while (rn->next && rn->next != after) 
            {
                rn = rn->next;
            }

            tail = rn;
            tail->next = nullptr;
        }
    }
    else
    {
        Node* pos = const_cast<Node*>(position.current);
        Node* prev = pos->prev;

        if (prev) 
        {
            prev->next = f; 
        }
        else 
        {
            head = f;
        }

        f->prev = prev;

        Node* rn = f; 
        while (rn->next && rn->next != after) 
        {
            rn = rn->next;
        }
        rn->next = pos;
        pos->prev = rn;
    }
    size_of_list += cnt;
}

template<typename T>
void list<T>::splice(const_iterator position, list&& x, const_iterator first, const_iterator last)
{
    splice(position, x, first, last);
}

template<typename T>
void list<T>::sort()
{
    if (!head || !head->next) return;

    std::vector<Node*> runs;
    Node* cur = head;
    while (cur)
    {
        Node* start = cur;

        while (cur->next && !(cur->next->value < cur->value)) 
        {
            cur = cur->next;
        }

        Node* next = cur->next;
        cur->next = nullptr;

        if (next) 
        {
            next->prev = nullptr;
        }

        runs.push_back(start);
        cur = next;
    }
    auto merge_two = [&](Node* a, Node* b)->Node*
    {
        Node dummy(T());
        Node* last = &dummy;
        while (a && b)
        {
            if (!(b->value < a->value))
            {
                Node* na = a->next;
                last->next = a; a->prev = last == &dummy ? nullptr : last; 
                last = a; 
                a = na;
            }
            else
            {
                Node* nb = b->next;
                last->next = b; b->prev = last == &dummy ? nullptr : last; 
                last = b; 
                b = nb;
            }
        }
        Node* rem = a ? a : b;
        while (rem)
        {
            Node* nr = rem->next;
            last->next = rem; rem->prev = last == &dummy ? nullptr : last; 
            last = rem; 
            
            rem = nr;
        }
        return dummy.next;
    };

    while (runs.size() > 1)
    {
        std::vector<Node*> next_runs;
        for (size_t i = 0; i + 1 < runs.size(); i += 2)
        {
            Node* m = merge_two(runs[i], runs[i+1]);
            Node* t = m; 

            while (t->next) 
            {
                t = t->next; 
                t->next = nullptr;
            }
            next_runs.push_back(m);
        }

        if (runs.size() % 2 == 1) 
        {
            next_runs.push_back(runs.back());
        }
        runs.swap(next_runs);
    }

    head = runs.front();
    Node* p = head; p->prev = nullptr; 
    
    while (p->next) 
    { 
        p->next->prev = p; p = p->next; 
    }
    tail = p;
}

template<typename T>
bool operator==(const list<T>& a, const list<T>& b)
{
    if (a.size() != b.size()) return false;
    return std::equal(a.begin(), a.end(), b.begin());
}

template<typename T>
bool operator!=(const list<T>& a, const list<T>& b)
{
    return !(a == b);
}

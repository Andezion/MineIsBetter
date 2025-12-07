#pragma once

#include <stdexcept>

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
    using reverse_iterator = std::reverse_iterator<struct iterator>;
    using const_reverse_iterator = std::reverse_iterator<struct const_iterator>;
    struct const_iterator
    {
        const Node* current;

        explicit const_iterator(const Node* p = nullptr) : current(p) {}

        const T& operator*() const noexcept { return current->value; }
        const_iterator& operator++() noexcept { if (current) current = current->next; return *this; }
        bool operator!=(const const_iterator& other) const noexcept { return current != other.current; }
    };
    struct iterator
    {
        Node* current;

        explicit iterator(Node* p = nullptr) : current(p) {}

        T& operator*() noexcept { return current->value; }
        iterator& operator++() noexcept { if (current) current = current->next; return *this; }
        bool operator!=(const iterator& other) const noexcept { return current != other.current; }
    };

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
    void clear() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    void emplace_back(const T& value);
    void emplace_front(const T& value);
    bool empty() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    T& front();
    const T& front() const;

    iterator insert (const_iterator position, const T& val);

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
    ~list();
};

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
    if (this == &other) return *this;
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
        // assigning to self: nothing to do
    }
}

template<typename T>
void list<T>::assign(std::initializer_list<T> init)
{
    clear();
    for (const T& v : init) push_back(v);
}

template<typename T>
void list<T>::assign(size_t size, const T &value)
{
    clear();
    for (size_t i = 0; i < size; ++i) push_back(value);
}

template<typename T>
void list<T>::assign(T *first, T *last)
{
    clear();
    for (T* p = first; p != last; ++p) push_back(*p);
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
list<T>::~list()
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

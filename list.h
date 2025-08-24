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
    this->assign(size);
}

template<typename T>
list<T>::list(const list &other)
{
    this->head = nullptr;
    this->tail = nullptr;
    this->size_of_list = 0;
    this->assign(other);
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
    this->assign(init);
}

template<typename T>
list<T> & list<T>::operator=(const list &other)
{
    if (this != &other)
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
    }
    this->head = other.head;
    this->tail = other.tail;
    this->size_of_list = other.size_of_list;
    other.head = nullptr;
    other.tail = nullptr;
    other.size_of_list = 0;
    return *this;
}

template<typename T>
list<T> & list<T>::operator=(list &&other) noexcept
{
    if (this != &other)
    {
        this->head = other.head;
        this->tail = other.tail;
        this->size_of_list = other.size_of_list;
        other.head = nullptr;
        other.tail = nullptr;
        other.size_of_list = 0;
        return *this;
    }
    return *this;
}

template<typename T>
void list<T>::assign(const list& other)
{
    if (this == &other)
    {
        return;
    }

    this->~list();
    head = tail = nullptr;
    size_of_list = 0;

    for (Node* cur = other.head; cur; cur = cur->next)
    {
        Node* new_node = new Node(cur->value, tail, nullptr);
        if (!head)
        {
            head = new_node;
        }
        if (tail)
        {
            tail->next = new_node;
        }
        tail = new_node;
        ++size_of_list;
    }
}

template<typename T>
void list<T>::assign(list &&other) noexcept
{
    if (this != &other)
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
    else
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
    }
}

template<typename T>
void list<T>::assign(std::initializer_list<T> init)
{
    if (this != &init)
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
        this->head = init.head;
        this->tail = init.tail;
        this->size_of_list = init.size_of_list;
        init.head = nullptr;
        init.tail = nullptr;
        init.size_of_list = 0;

    }
    else
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
    }
}

template<typename T>
void list<T>::assign(size_t size, const T &value)
{
    if (this != &value)
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
        this->size_of_list = 0;
        this->head = new Node(value);
        this->size_of_list = size;
    }
    else
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
    }
}

template<typename T>
void list<T>::assign(T *first, T *last)
{
    if (this != &first)
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
        this->size_of_list = 0;
        this->head = new Node(*first);
        this->tail = this->head;
        this->size_of_list = *last;
        this->size_of_list = *last;
        this->head->next = nullptr;
        this->tail->next = nullptr;
    }
    else
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size_of_list = 0;
    }
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

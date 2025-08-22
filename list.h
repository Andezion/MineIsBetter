#pragma once
#include <stdexcept>

template<typename T>
class list {
    struct Node {
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
    list() : head(nullptr), tail(nullptr), size_of_list(0) {}
    list(const list& other) : head(nullptr), tail(nullptr), size_of_list(0) {
        for (Node* p = other.head; p; p = p->next) push_back(p->value);
    }
    list(list&& other) noexcept : head(other.head), tail(other.tail), size_of_list(other.size_of_list) {
        other.head = other.tail = nullptr;
        other.size_of_list = 0;
    }
    list(std::initializer_list<T> init) : head(nullptr), tail(nullptr), size_of_list(0) {
        for (auto& v : init) push_back(v);
    }
    ~list() { clear(); }

    void push_back(const T& value) {
        Node* node = new Node(value, tail, nullptr);
        if (tail) tail->next = node; else head = node;
        tail = node;
        ++size_of_list;
    }

    void push_front(const T& value) {
        Node* node = new Node(value, nullptr, head);
        if (head) head->prev = node; else tail = node;
        head = node;
        ++size_of_list;
    }

    void pop_back() {
        if (!tail) throw std::out_of_range("empty");
        const Node* old = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr; else head = nullptr;
        delete old;
        --size_of_list;
    }

    void pop_front() {
        if (!head) throw std::out_of_range("empty");
        Node* old = head;
        head = head->next;
        if (head) head->prev = nullptr; else tail = nullptr;
        delete old;
        --size_of_list;
    }

    bool empty() const noexcept { return size_of_list == 0; }
    size_t size() const noexcept { return size_of_list; }

    T& front() {
        if (!head) throw std::out_of_range("empty");
        return head->value;
    }
    T& back() {
        if (!tail) throw std::out_of_range("empty");
        return tail->value;
    }

    void clear() {
        Node* p = head;
        while (p) {
            Node* next = p->next;
            delete p;
            p = next;
        }
        head = tail = nullptr;
        size_of_list = 0;
    }
};

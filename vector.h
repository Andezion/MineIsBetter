#pragma once

#include <limits>
#include <stdexcept>
#include <utility>

template<typename T>
class vector
{
    T * storage;
    size_t size_of_vector{};
    size_t capacity_of_vector{};

public:
    vector();
    explicit vector(size_t size);
    vector(size_t size, const T& value);

    ~vector();

    void assign(size_t size, const T& value);
    void assign(T* first, T* last);
    void assign(std::initializer_list<T> list);
    const T& at(size_t index) const;
    const T& back() const;
    T* begin() const noexcept;
    size_t capacity() const noexcept;
    const T* cbegin() const noexcept;
    const T* cend() const noexcept;
    void clear() noexcept;
    const T* crbegin() const noexcept;
    const T* crend() const noexcept;
    T* data() const noexcept;
    T* emplace(const T* position, const T& value);
    T* emplace_back(const T& value);
    bool empty() const noexcept;
    T* end() const noexcept;
    T* erase(T* first, T* last);
    T* erase(T* first);
    T& front() const;
    T* insert(size_t index, const T& value);
    T* insert(size_t index, const T* first, const T* last);
    T* insert(size_t index, std::initializer_list<T> list);
    T* insert(size_t index, T&& value);

    static size_t max_size() noexcept;
    T& operator[](size_t index) const;
    vector& operator=(const vector& other);
    vector& operator=(vector&& other) noexcept;
    vector& operator=(std::initializer_list<T> list);
    void pop_back();
    void push_back(const T& value);
    void push_back(T&& value);
    const T* rbegin() const noexcept;
    const T* rend() const noexcept;
    void reserve(size_t new_capacity);
    void resize(size_t new_size);
    void resize(size_t new_size, const T& value);
    void shrink_to_fit();
    size_t size() const noexcept;
    void swap(vector& other) noexcept;

    template <class InputIterator> void assign(InputIterator first, InputIterator last);
    void assign (T n, const T& val);
    reference at (T n);
    const_reference at (T n) const;
    reference back();
    const_reference back() const;
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    size_type capacity() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    void clear() noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    value_type* data() noexcept;
    const value_type* data() const noexcept;
    template <class... Args>iterator emplace(const_iterator position, Args&&... args);
    template <class... Args>  void emplace_back(Args&&... args);
    bool empty() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    iterator erase (const_iterator position);
    iterator erase (const_iterator first, const_iterator last);
    reference front();
    const_reference front() const;
};

template<typename T>
vector<T>::vector()
{
    storage = nullptr;
    size_of_vector = 0;
    capacity_of_vector = 0;
}

template<typename T>
vector<T>::vector(const size_t size)
{
    storage = new T[size];
    size_of_vector = size;
    capacity_of_vector = size;
}

template<typename T>
vector<T>::vector(const size_t size, const T &value)
{
    storage = new T[size];
    size_of_vector = size;
    capacity_of_vector = size;
    for (size_t i = 0; i < size; i++)
    {
        storage[i] = value;
    }
}

template<typename T>
vector<T>::~vector()
{
    if (storage)
    {
        delete[] storage;
        storage = nullptr;
        size_of_vector = 0;
        capacity_of_vector = 0;
    }
    else
    {
        size_of_vector = 0;
        capacity_of_vector = 0;
    }
}

template<typename T>
void vector<T>::assign(const size_t size, const T &value)
{
    delete[] storage;

    storage = new T[size];

    for (size_t i = 0; i < size; i++)
    {
        storage[i] = value;
    }

    size_of_vector = size;
    capacity_of_vector = size;
}

template<typename T>
void vector<T>::assign(T *first, T *last)
{
    const size_t n = last - first;

    delete[] storage;
    storage = new T[n];

    for (size_t i = 0; i < n; i++)
    {
        storage[i] = first[i];
    }

    size_of_vector = n;
    capacity_of_vector = n;
}

template<typename T>
void vector<T>::assign(std::initializer_list<T> list)
{
    const size_t n = list.size();

    delete[] storage;
    storage = new T[n];

    size_t i = 0;
    for (const T& val : list)
    {
        storage[i++] = val;
    }

    size_of_vector = n;
    capacity_of_vector = n;
}


template<typename T>
const T & vector<T>::at(size_t index) const
{
    if (index >= size_of_vector)
    {
        throw std::out_of_range("Index is incorrect");
    }
    return storage[index];
}

template<typename T>
const T & vector<T>::back() const
{
    if (size_of_vector == 0)
    {
        throw std::out_of_range("Vector is empty");
    }
    return storage[size_of_vector - 1];
}

template<typename T>
T * vector<T>::begin() const noexcept
{
    return storage;
}

template<typename T>
size_t vector<T>::capacity() const noexcept
{
    return capacity_of_vector;
}

template<typename T>
const T * vector<T>::cbegin() const noexcept
{
    return storage;
}

template<typename T>
const T * vector<T>::cend() const noexcept
{
    return storage + size_of_vector;
}

template<typename T>
void vector<T>::clear() noexcept
{
    size_of_vector = 0;
}

template<typename T>
const T * vector<T>::crbegin() const noexcept
{
    return storage + size_of_vector;
}

template<typename T>
const T * vector<T>::crend() const noexcept
{
    return storage;
}

template<typename T>
T * vector<T>::data() const noexcept
{
    return storage;
}

template<typename T>
T* vector<T>::emplace(const T* position, const T& value)
{
    size_t pos_index = position - storage;

    if (size_of_vector >= capacity_of_vector)
    {
        if (capacity_of_vector == 0)
        {
            capacity_of_vector = 1;
        }
        else
        {
            capacity_of_vector = capacity_of_vector * 2;
        }
        T* new_storage = new T[capacity_of_vector];

        for (size_t i = 0; i < pos_index; i++)
        {
            new_storage[i] = storage[i];
        }

        new_storage[pos_index] = value;

        for (size_t i = pos_index; i < size_of_vector; ++i)
        {
            new_storage[i + 1] = storage[i];
        }

        delete[] storage;
        storage = new_storage;
    }
    else
    {
        for (size_t i = size_of_vector; i > pos_index; i--)
        {
            storage[i] = storage[i - 1];
        }

        storage[pos_index] = value;
    }

    ++size_of_vector;
    return storage + pos_index;
}



template<typename T>
T * vector<T>::emplace_back(const T &value)
{
    if (size_of_vector >= capacity_of_vector)
    {
        if (capacity_of_vector == 0)
        {
            capacity_of_vector = 1;
        }
        else
        {
            capacity_of_vector = capacity_of_vector * 2;
        }

        T * new_storage = new T[capacity_of_vector];
        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = storage[i];
        }
        delete[] storage;
        storage = new_storage;
    }
    storage[size_of_vector++] = value;
    return &storage[size_of_vector - 1];
}

template<typename T>
bool vector<T>::empty() const noexcept
{
    return size_of_vector == 0;
}

template<typename T>
T * vector<T>::end() const noexcept
{
    return storage + size_of_vector;
}

template<typename T>
T* vector<T>::erase(T* first, T* last)
{
    if (first < storage || last > storage + size_of_vector || first > last)
    {
        throw std::out_of_range("Invalid erase range");
    }

    const size_t erase_count = last - first;
    T* new_end = first;

    for (T* p = last; p < storage + size_of_vector; ++p, ++new_end) {
        *new_end = std::move(*p);
    }

    size_of_vector -= erase_count;
    return first;
}

template<typename T>
T* vector<T>::erase(T* first)
{
    return erase(first, first + 1);
}

template<typename T>
T & vector<T>::front() const
{
    if (size_of_vector == 0)
    {
        throw std::out_of_range("Vector is empty");
    }
    return storage[0];
}

template<typename T>
T* vector<T>::insert(size_t index, const T& value)
{
    if (index > size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }

    if (size_of_vector >= capacity_of_vector)
    {
        if (capacity_of_vector == 0)
        {
            capacity_of_vector = 1;
        }
        else
        {
            capacity_of_vector = capacity_of_vector * 2;
        }
    }

    T * new_storage = new T[capacity_of_vector];

    for (size_t i = 0; i < index; i++)
    {
        new_storage[i] = storage[i];
    }

    new_storage[index] = value;

    for (size_t i = index; i < size_of_vector; i++)
    {
        new_storage[i + 1] = storage[i];
    }

    delete[] storage;
    storage = new_storage;
    ++size_of_vector;

    return &storage[index];
}

template<typename T>
T* vector<T>::insert(size_t index, const T* first, const T* last)
{
    if (index > size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }

    const size_t count = last - first;
    const size_t new_size = size_of_vector + count;

    if (new_size > capacity_of_vector)
    {
        while (capacity_of_vector < new_size)
        {
            capacity_of_vector = (capacity_of_vector == 0) ? 1 : capacity_of_vector * 2;
        }

        T* new_storage = new T[capacity_of_vector];

        for (size_t i = 0; i < index; i++)
        {
            new_storage[i] = std::move(storage[i]);
        }

        size_t pos = index;
        while (first != last)
        {
            new_storage[pos++] = *first++;
        }

        for (size_t i = index; i < size_of_vector; i++)
        {
            new_storage[pos++] = std::move(storage[i]);
        }

        delete[] storage;
        storage = new_storage;
    }
    else
    {
        for (size_t i = size_of_vector; i > index; i--)
        {
            storage[i + count - 1] = std::move(storage[i - 1]);
        }

        for (size_t i = 0; i < count; i++)
        {
            storage[index + i] = first[i];
        }
    }

    size_of_vector = new_size;
    return storage + index;
}


template<typename T>
T* vector<T>::insert(size_t index, std::initializer_list<T> list)
{
    if (index > size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }

    const size_t new_size = size_of_vector + list.size();
    if (new_size > capacity_of_vector)
    {
        while (capacity_of_vector < new_size)
        {
            capacity_of_vector = (capacity_of_vector == 0) ? 1 : capacity_of_vector * 2;
        }
    }

    T* new_storage = new T[capacity_of_vector];

    for (size_t i = 0; i < index; i++)
    {
        new_storage[i] = storage[i];
    }

    size_t pos = index;
    for (const auto& elem : list)
    {
        new_storage[pos++] = elem;
    }

    for (size_t i = index; i < size_of_vector; i++)
    {
        new_storage[pos++] = storage[i];
    }

    delete[] storage;
    storage = new_storage;
    size_of_vector = new_size;

    return storage + index;
}

template<typename T>
T* vector<T>::insert(size_t index, T&& value)
{
    if (index > size_of_vector)
    {
        throw std::out_of_range("Index out of range");
    }

    if (size_of_vector >= capacity_of_vector)
    {
        capacity_of_vector = (capacity_of_vector == 0) ? 1 : capacity_of_vector * 2;

        T* new_storage = new T[capacity_of_vector];

        for (size_t i = 0; i < index; i++)
        {
            new_storage[i] = std::move(storage[i]);
        }

        new_storage[index] = std::move(value);

        for (size_t i = index; i < size_of_vector; i++)
        {
            new_storage[i + 1] = std::move(storage[i]);
        }

        delete[] storage;
        storage = new_storage;
    }
    else
    {
        for (size_t i = size_of_vector; i > index; --i)
        {
            storage[i] = std::move(storage[i - 1]);
        }
        storage[index] = std::move(value);
    }

    ++size_of_vector;
    return &storage[index];
}


template<typename T>
size_t vector<T>::max_size() noexcept
{
    return std::numeric_limits<size_t>::max() / sizeof(T);
}

template<typename T>
T & vector<T>::operator[](size_t index) const
{
    if (index >= size_of_vector)
    {
        throw std::out_of_range("Index is incorrect");
    }
    return *(storage + index);
}

template<typename T>
vector<T> & vector<T>::operator=(const vector &other)
{
    if (this != &other)
    {
        clear();
        reserve(other.size_of_vector);
        for (size_t i = 0; i < other.size_of_vector; i++)
        {
            push_back(other[i]);
        }
    }
    return *this;
}

template<typename T>
vector<T> & vector<T>::operator=(vector &&other)
 noexcept {
    if (this != &other)
    {
        delete[] storage;

        storage = other.storage;
        size_of_vector = other.size_of_vector;
        capacity_of_vector = other.capacity_of_vector;

        other.storage = nullptr;
        other.size_of_vector = 0;
        other.capacity_of_vector = 0;
    }
    return *this;
}

template<typename T>
vector<T> & vector<T>::operator=(std::initializer_list<T> list)
{
    delete[] storage;

    capacity_of_vector = list.size();
    size_of_vector = list.size();
    storage = new T[capacity_of_vector];

    size_t i = 0;
    for (const auto& elem : list)
    {
        storage[i++] = elem;
    }

    return *this;
}


template<typename T>
void vector<T>::pop_back()
{
    if (size_of_vector == 0)
    {
        throw std::out_of_range("Vector is empty");
    }
    storage[size_of_vector - 1].~T();
    size_of_vector--;
}

template<typename T>
void vector<T>::push_back(const T &value)
{
    if (size_of_vector >= capacity_of_vector)
    {
        if (capacity_of_vector == 0)
        {
            capacity_of_vector = 1;
        }
        else
        {
            capacity_of_vector = capacity_of_vector * 2;
        }

        T * new_storage = new T[capacity_of_vector];
        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = storage[i];
        }
        delete[] storage;
        storage = new_storage;
    }
    storage[size_of_vector++] = value;
}

template<typename T>
void vector<T>::push_back(T &&value)
{
    if (size_of_vector >= capacity_of_vector)
    {
        if (capacity_of_vector == 0)
        {
            capacity_of_vector = 1;
        }
        else
        {
            capacity_of_vector = capacity_of_vector * 2;
        }

        T * new_storage = new T[capacity_of_vector];
        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = storage[i];
        }
        delete[] storage;
        storage = new_storage;
    }
    storage[size_of_vector++] = std::move(value);
}

template<typename T>
const T * vector<T>::rbegin() const noexcept
{
    return storage + size_of_vector;
}

template<typename T>
const T * vector<T>::rend() const noexcept
{
    return storage;
}

template<typename T>
void vector<T>::reserve(const size_t new_capacity)
{
    if (new_capacity > capacity_of_vector)
    {
        T* new_storage = new T[new_capacity];
        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = std::move(storage[i]);
        }
        delete[] storage;
        storage = new_storage;
        capacity_of_vector = new_capacity;
    }
}

template<typename T>
void vector<T>::resize(const size_t new_size)
{
    if (new_size > capacity_of_vector)
    {
        const size_t new_capacity = std::max(new_size, capacity_of_vector * 2);
        T* new_storage = new T[new_capacity];

        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = std::move(storage[i]);
        }

        delete[] storage;
        storage = new_storage;
        capacity_of_vector = new_capacity;
    }

    for (size_t i = size_of_vector; i < new_size; i++)
    {
        storage[i] = T{};
    }

    size_of_vector = new_size;
}

template<typename T>
void vector<T>::resize(size_t new_size, const T &value)
{
    if (new_size > capacity_of_vector)
    {
        const size_t new_capacity = std::max(new_size, capacity_of_vector * 2);
        T* new_storage = new T[new_capacity];

        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = std::move(storage[i]);
        }
        delete[] storage;
        storage = new_storage;
        capacity_of_vector = new_capacity;
    }

    for (size_t i = size_of_vector; i < new_size; i++)
    {
        storage[i] = value;
    }

    size_of_vector = new_size;
}


template<typename T>
void vector<T>::shrink_to_fit()
{
    if (size_of_vector < capacity_of_vector)
    {
        T* new_storage = new T[size_of_vector];
        for (size_t i = 0; i < size_of_vector; i++)
        {
            new_storage[i] = storage[i];
        }
        delete[] storage;
        storage = new_storage;
        capacity_of_vector = size_of_vector;
    }
}

template<typename T>
size_t vector<T>::size() const noexcept
{
    return size_of_vector;
}

template<typename T>
void vector<T>::swap(vector &other) noexcept
{
    std::swap(storage, other.storage);
    std::swap(size_of_vector, other.size_of_vector);
    std::swap(capacity_of_vector, other.capacity_of_vector);
}








#pragma once

#include <functional>
#include <memory>
#include <iterator>
#include <initializer_list>
#include <utility>
#include <algorithm>

template<
    class T,
    class Compare = std::less<T>,
    class Allocator = std::allocator<T>>
class set
{
public:
    using key_type = T;
    using value_type = T;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using difference_type = std::ptrdiff_t;
    using size_type = std::size_t;

private:
    struct Node
    {
        value_type value;
        Node* left;
        Node* right;
        Node* parent;
        bool is_black;

        explicit Node(const value_type& val)
            : value(val), left(nullptr), right(nullptr), parent(nullptr), is_black(false) {}
    };

    using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using NodeAllocTraits = std::allocator_traits<NodeAllocator>;

public:
    class iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        iterator() : node_(nullptr), tree_(nullptr) {}

        reference operator*() const;
        pointer operator->() const;

        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);

        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;

    private:
        friend class set;
        iterator(Node* node, const set* tree) : node_(node), tree_(tree) {}

        Node* node_;
        const set* tree_;
    };

    class const_iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() : node_(nullptr), tree_(nullptr) {}
            explicit const_iterator(const iterator& it);

        reference operator*() const;
        pointer operator->() const;

        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

    private:
        friend class set;
        const_iterator(Node* node, const set* tree) : node_(node), tree_(tree) {}

        Node* node_;
        const set* tree_;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    set();

    explicit set(const Compare& comp, const Allocator& alloc = Allocator());
    explicit set(const Allocator& alloc);

    template<class InputIt>
    set(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator());

    template<class InputIt>
    set(InputIt first, InputIt last, const Allocator& alloc);

    set(const set& other);
    set(const set& other, const Allocator& alloc);
    set(set&& other) noexcept;
    set(set&& other, const Allocator& alloc);

    set(std::initializer_list<value_type> init, const Compare& comp = Compare(), const Allocator& alloc = Allocator());
    set(std::initializer_list<value_type> init, const Allocator& alloc);

    ~set();

    set& operator=(const set& other);
    set& operator=(set&& other) noexcept;
    set& operator=(std::initializer_list<value_type> ilist);

    allocator_type get_allocator() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;
    [[nodiscard]] size_type max_size() const noexcept;

    void clear() noexcept;

    std::pair<iterator, bool> insert(const value_type& value);
    std::pair<iterator, bool> insert(value_type&& value);
    iterator insert(const_iterator hint, const value_type& value);
    iterator insert(const_iterator hint, value_type&& value);

    template<class InputIt>
    void insert(InputIt first, InputIt last);

    void insert(std::initializer_list<value_type> ilist);

    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args);

    template<class... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args);

    iterator erase(iterator pos);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);
    size_type erase(const key_type& key);

    void swap(set& other) noexcept;

    size_type count(const key_type& key) const;

    template<class K>
    size_type count(const K& x) const;

    iterator find(const key_type& key);
    const_iterator find(const key_type& key) const;

    template<class K>
    iterator find(const K& x);

    template<class K>
    const_iterator find(const K& x) const;

    bool contains(const key_type& key) const;

    template<class K>
    bool contains(const K& x) const;

    std::pair<iterator, iterator> equal_range(const key_type& key);
    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const;

    template<class K>
    std::pair<iterator, iterator> equal_range(const K& x);

    template<class K>
    std::pair<const_iterator, const_iterator> equal_range(const K& x) const;

    iterator lower_bound(const key_type& key);
    const_iterator lower_bound(const key_type& key) const;

    template<class K>
    iterator lower_bound(const K& x);

    template<class K>
    const_iterator lower_bound(const K& x) const;

    iterator upper_bound(const key_type& key);
    const_iterator upper_bound(const key_type& key) const;

    template<class K>
    iterator upper_bound(const K& x);

    template<class K>
    const_iterator upper_bound(const K& x) const;

    key_compare key_comp() const;
    value_compare value_comp() const;

private:
    Node* create_node(const value_type& value);
    Node* create_node(value_type&& value);
    void destroy_node(Node* node);
    void destroy_tree(Node* node);

    Node* copy_tree(Node* other_node, Node* parent);

    Node* find_node(const key_type& key) const;

    std::pair<Node*, bool> insert_node(const value_type& value);
    std::pair<Node*, bool> insert_node(value_type&& value);

    void erase_node(Node* node);

    void rotate_left(Node* node);
    void rotate_right(Node* node);
    void fix_insert(Node* node);
    void fix_erase(Node* node, Node* parent);

    static Node* minimum(Node* node);
    static Node* maximum(Node* node);
    Node* successor(Node* node) const;
    Node* predecessor(Node* node) const;

    void transplant(Node* u, Node* v);

    Node* root_;
    Node* leftmost_;
    Node* rightmost_;
    size_type size_;
    Compare comp_;
    NodeAllocator alloc_;
};

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator::reference set<T, Compare, Allocator>::iterator::operator*() const
{
    return node_->value;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator::pointer set<T, Compare, Allocator>::iterator::operator->() const
{
    return &node_->value;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator & set<T, Compare, Allocator>::iterator::operator++()
{
    if (node_->right)
    {
        node_ = node_->right;
        while (node_->left)
        {
            node_ = node_->left;
        }
    }
    else
    {
        Node* parent = node_->parent;
        while (parent && node_ == parent->right)
        {
            node_ = parent;
            parent = parent->parent;
        }
        node_ = parent;
    }
    return *this;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::iterator::operator++(int)
{
    iterator tmp = *this;
    ++*this;
    return tmp;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator & set<T, Compare, Allocator>::iterator::operator--()
{
    if (!node_)
    {
        node_ = tree_->root_ ? maximum(tree_->root_) : nullptr;
        return *this;
    }

    if (node_->left)
    {
        node_ = node_->left;
        while (node_->right)
        {
            node_ = node_->right;
        }
    }
    else
    {
        Node* parent = node_->parent;
        while (parent && node_ == parent->left)
        {
            node_ = parent;
            parent = parent->parent;
        }
        node_ = parent;
    }
    return *this;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::iterator::operator--(int)
{
    iterator tmp = *this;
    --*this;
    return tmp;
}

template<class T, class Compare, class Allocator>
bool set<T, Compare, Allocator>::iterator::operator==(const iterator &other) const
{
    return node_ == other.node_;
}

template<class T, class Compare, class Allocator>
bool set<T, Compare, Allocator>::iterator::operator!=(const iterator &other) const
{
    return node_ != other.node_;
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::const_iterator::const_iterator(const iterator &it)
    : node_(it.node_), tree_(it.tree_) {}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator::reference
set<T, Compare, Allocator>::const_iterator::operator*() const
{
    return node_->value;
}
template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator::pointer
set<T, Compare, Allocator>::const_iterator::operator->() const
{
    return &node_->value;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator & set<T, Compare, Allocator>::const_iterator::operator++()
{
    if (node_->right)
    {
        node_ = node_->right;
        while (node_->left)
        {
            node_ = node_->left;
        }
    }
    else
    {
        Node* p = node_->parent;
        while (p && node_ == p->right)
        {
            node_ = p;
            p = p->parent;
        }
        node_ = p;
    }
    return *this;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::const_iterator::operator++(int)
{
    auto tmp = *this;
    ++*this;
    return tmp;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator & set<T, Compare, Allocator>::const_iterator::operator--()
{
    if (!node_)
    {
        node_ = tree_->root_ ? maximum(tree_->root_) : nullptr;
        return *this;
    }

    if (node_->left)
    {
        node_ = node_->left;
        while (node_->right)
        {
            node_ = node_->right;
        }
    }
    else
    {
        Node* p = node_->parent;
        while (p && node_ == p->left)
        {
            node_ = p;
            p = p->parent;
        }
        node_ = p;
    }
    return *this;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::const_iterator::operator--(int)
{
    auto tmp = *this;
    --*this;
    return tmp;
}

template<class T, class Compare, class Allocator>
bool set<T, Compare, Allocator>::const_iterator::operator==(const const_iterator &other) const
{
    return node_ == other.node_;
}

template<class T, class Compare, class Allocator>
bool set<T, Compare, Allocator>::const_iterator::operator!=(const const_iterator &other) const
{
    return node_ != other.node_;
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set()
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(Compare())
    , alloc_(NodeAllocator())
{

}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(const Compare& comp, const Allocator& alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(comp)
    , alloc_(alloc)
{
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(const Allocator& alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(Compare())
    , alloc_(alloc)
{
}

template<class T, class Compare, class Allocator>
template<class InputIt>
set<T, Compare, Allocator>::set(InputIt first, InputIt last, const Compare& comp, const Allocator& alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(comp)
    , alloc_(alloc)
{
    insert(first, last);
}

template<class T, class Compare, class Allocator>
template<class InputIt>
set<T, Compare, Allocator>::set(InputIt first, InputIt last, const Allocator& alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(Compare())
    , alloc_(alloc)
{
    insert(first, last);
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(std::initializer_list<value_type> init, const Compare& comp, const Allocator& alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(comp)
    , alloc_(alloc)
{
    insert(init);
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(std::initializer_list<value_type> init, const Allocator& alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(Compare())
    , alloc_(alloc)
{
    insert(init);
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(const set &other)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(other.comp_)
    , alloc_(other.alloc_)
{
    for (const T& val : other)
    {
        insert(val);
    }
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(const set &other, const Allocator &alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(other.comp_)
    , alloc_(alloc)
{
    for (const T& val : other)
    {
        insert(val);
    }
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(set &&other) noexcept
    : root_(other.root_)
    , leftmost_(other.leftmost_)
    , rightmost_(other.rightmost_)
    , size_(other.size_)
    , comp_(std::move(other.comp_))
    , alloc_(std::move(other.alloc_))
{
    other.root_ = nullptr;
    other.leftmost_ = nullptr;
    other.rightmost_ = nullptr;
    other.size_ = 0;
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(set &&other, const Allocator &alloc)
    : root_(nullptr)
    , leftmost_(nullptr)
    , rightmost_(nullptr)
    , size_(0)
    , comp_(std::move(other.comp_))
    , alloc_(alloc)
{
    if (alloc == other.alloc_)
    {
        root_ = other.root_;
        leftmost_ = other.leftmost_;
        rightmost_ = other.rightmost_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.leftmost_ = nullptr;
        other.rightmost_ = nullptr;
        other.size_ = 0;
    }
    else
    {
        for (const T& val : other)
        {
            insert(val);
        }
        other.clear();
    }
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::~set()
{
    clear();
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator> & set<T, Compare, Allocator>::operator=(const set &other)
{
    if (this == &other)
    {
        return *this;
    }

    clear();

    alloc_ = other.alloc_;
    comp_ = other.comp_;

    for (const auto &value : other)
    {
        insert(value);
    }

    return *this;
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator> & set<T, Compare, Allocator>::operator=(set &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    clear();

    alloc_ = std::move(other.alloc_);
    comp_ = std::move(other.comp_);
    size_ = other.size_;
    root_ = other.root_;
    leftmost_ = other.leftmost_;
    rightmost_ = other.rightmost_;

    other.size_ = 0;
    other.root_ = nullptr;
    other.leftmost_ = nullptr;
    other.rightmost_ = nullptr;

    return *this;
}

template<class T, class Compare, class Allocator>
set<T, Compare, Allocator> & set<T, Compare, Allocator>::operator=(std::initializer_list<value_type> ilist)
{
    clear();
    for (const auto &value : ilist)
    {
        insert(value);
    }
    return *this;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::begin() noexcept
{
    return root_ ? iterator(minimum(root_), this) : iterator(nullptr, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::begin() const noexcept
{
    return root_ ? const_iterator(minimum(root_), this) : const_iterator(nullptr, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::cbegin() const noexcept
{
    return root_ ? const_iterator(minimum(root_), this) : const_iterator(nullptr, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::end() noexcept
{
    return iterator(nullptr, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::end() const noexcept
{
    return const_iterator(nullptr, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::cend() const noexcept
{
    return const_iterator(nullptr, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::reverse_iterator set<T, Compare, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_reverse_iterator set<T, Compare, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_reverse_iterator set<T, Compare, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::reverse_iterator set<T, Compare, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_reverse_iterator set<T, Compare, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(cend());
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_reverse_iterator set<T, Compare, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(cend());
}

template<class T, class Compare, class Allocator>
bool set<T, Compare, Allocator>::empty() const noexcept
{
    return size_ == 0;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::size_type set<T, Compare, Allocator>::size() const noexcept
{
    return size_;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::size_type set<T, Compare, Allocator>::max_size() const noexcept
{
    return std::allocator_traits<NodeAllocator>::max_size(alloc_);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::allocator_type
set<T, Compare, Allocator>::get_allocator() const noexcept
{
    return Allocator();
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::clear() noexcept
{
    destroy_tree(root_);
    root_ = leftmost_ = rightmost_ = nullptr;
    size_ = 0;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::size_type set<T, Compare, Allocator>::count(const key_type &key) const
{
    return find(key) != end() ? 1 : 0;
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::size_type set<T, Compare, Allocator>::count(const K &x) const
{
    return find(x) != end() ? 1 : 0;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::key_compare set<T, Compare, Allocator>::key_comp() const
{
    return comp_;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::value_compare set<T, Compare, Allocator>::value_comp() const
{
    return comp_;
}

template<class T, class Compare, class Allocator>
std::pair<typename set<T, Compare, Allocator>::iterator, typename set<T, Compare, Allocator>::iterator>
set<T, Compare, Allocator>::equal_range(const key_type &key)
{
    return { lower_bound(key), upper_bound(key) };
}

template<class T, class Compare, class Allocator>
std::pair<typename set<T, Compare, Allocator>::const_iterator, typename set<T, Compare, Allocator>::const_iterator>
set<T, Compare, Allocator>::equal_range(const key_type &key) const
{
    return { lower_bound(key), upper_bound(key) };
}

template<class T, class Compare, class Allocator>
template<class K>
std::pair<typename set<T, Compare, Allocator>::iterator, typename set<T, Compare, Allocator>::iterator>
set<T, Compare, Allocator>::equal_range(const K &x)
{
    return { lower_bound(x), upper_bound(x) };
}

template<class T, class Compare, class Allocator>
template<class K>
std::pair<typename set<T, Compare, Allocator>::const_iterator, typename set<T, Compare, Allocator>::const_iterator>
set<T, Compare, Allocator>::equal_range(const K &x) const
{
    return { lower_bound(x), upper_bound(x) };
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::lower_bound(const K &x)
{
    if (root_ == nullptr) return end();
    auto it = iterator(minimum(root_), this);
    while (it != end() && comp_(*it, x)) ++it;
    return it;
}


template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::erase(const_iterator first, const_iterator last)
{
    auto it = first;
    while (it != last)
    {
        it = erase(it);
    }
    return iterator(last.node_, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::size_type
set<T, Compare, Allocator>::erase(const key_type &key)
{
    iterator it = find(key);
    if (it == end())
        return 0;

    erase(it);
    return 1;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::swap(set &other) noexcept
{
    if (this == &other)
        return;

    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);

    if constexpr (std::allocator_traits<Allocator>::propagate_on_container_swap::value)
    {
        std::swap(alloc_, other.alloc_);
    }
}

template<class T, class Compare, class Allocator>
bool set<T, Compare, Allocator>::contains(const key_type &key) const
{
    return find(key) != end();
}

template<class T, class Compare, class Allocator>
template<class K>
bool set<T, Compare, Allocator>::contains(const K &x) const
{
    return find(x) != end();
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::lower_bound(const key_type &key)
{
    if (root_ == nullptr)
    {
        return end();
    }

    auto it = iterator(minimum(root_), this);
    while (it != end() && comp_(*it, key))
    {
        ++it;
    }
    return it;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::erase(iterator pos)
{
    Node* node = pos.node_;
    if (!node) return iterator(nullptr, this);
    Node* next = successor(node);
    erase_node(node);
    return iterator(next, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::erase(const_iterator pos)
{
    Node* node = pos.node_;
    if (!node) return iterator(nullptr, this);
    Node* next = successor(node);
    erase_node(node);
    return iterator(next, this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::erase(const_iterator first, const_iterator last)
{
    auto it = first;
    while (it != last)
    {
        it = erase(it);
    }
    return iterator(last.node_, this);
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::const_iterator
set<T, Compare, Allocator>::lower_bound(const K &x) const
{
    if (root_ == nullptr)
    {
        return end();
    }

    auto it = const_iterator(minimum(root_), this);
    while (it != end() && comp_(*it, x))
    {
        ++it;
    }
    return it;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::upper_bound(const key_type &key)
{
    if (root_ == nullptr)
    {
        return end();
    }

    Node *current = root_;
    Node *result = nullptr;

    while (current != nullptr)
    {
        if (comp_(key, current->value))
        {
            result = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    if (result)
    {
        return iterator(result, this);
    }
    return end();
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator
set<T, Compare, Allocator>::upper_bound(const key_type &key) const
{
    if (root_ == nullptr)
    {
        return end();
    }

    Node *current = root_;
    Node *result = nullptr;

    while (current != nullptr)
    {
        if (comp_(key, current->value))
        {
            result = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    if (result)
    {
        return const_iterator(result, this);
    }
    return end();
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::iterator
set<T, Compare, Allocator>::upper_bound(const K &x)
{
    if (root_ == nullptr)
    {
        return end();
    }

    Node *current = root_;
    Node *result = nullptr;

    while (current != nullptr)
    {
        if (comp_(x, current->value))
        {
            result = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    if (result)
    {
        return iterator(result, this);
    }
    return end();
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::const_iterator
set<T, Compare, Allocator>::upper_bound(const K &x) const
{
    if (root_ == nullptr)
    {
        return end();
    }

    Node *current = root_;
    Node *result = nullptr;

    while (current != nullptr)
    {
        if (comp_(x, current->value))
        {
            result = current;
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    if (result)
    {
        return const_iterator(result, this);
    }
    return end();
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::create_node(const value_type &value)
{
    Node *node = alloc_.allocate(1);
    try
    {
        std::allocator_traits<NodeAllocator>::construct(alloc_, node, value);
    }
    catch (...)
    {
        alloc_.deallocate(node, 1);
        throw;
    }
    return node;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::create_node(value_type &&value)
{
    Node *node = alloc_.allocate(1);
    try
    {
        std::allocator_traits<NodeAllocator>::construct(alloc_, node, std::move(value));
    }
    catch (...)
    {
        alloc_.deallocate(node, 1);
        throw;
    }
    return node;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::destroy_node(Node *node)
{
    if (!node)
    {
        return;
    }

    std::allocator_traits<NodeAllocator>::destroy(alloc_, node);
    alloc_.deallocate(node, 1);
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::destroy_tree(Node *node)
{
    if (!node)
    {
        return;
    }

    destroy_tree(node->left);
    destroy_tree(node->right);
    destroy_node(node);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node* set<T, Compare, Allocator>::copy_tree(Node* other_node, Node* parent)
{
    if (!other_node)
        return nullptr;
    
    Node* new_node = create_node(other_node->value);
    new_node->is_black = other_node->is_black;
    new_node->parent = parent;
    
    try
    {
        new_node->left = copy_tree(other_node->left, new_node);
        new_node->right = copy_tree(other_node->right, new_node);
    }
    catch (...)
    {
        destroy_tree(new_node);
        throw;
    }
    
    return new_node;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node* set<T, Compare, Allocator>::find_node(const key_type& key) const
{
    Node* current = root_;
    while (current)
    {
        if (comp_(key, current->value))
            current = current->left;
        else if (comp_(current->value, key))
            current = current->right;
        else
            return current;
    }
    return nullptr;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::find(const key_type &key)
{
    Node* n = find_node(key);
    return n ? iterator(n, this) : end();
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::find(const key_type &key) const
{
    Node* n = find_node(key);
    return n ? const_iterator(n, this) : cend();
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::find(const K &x)
{
    Node* current = root_;
    while (current)
    {
        if (comp_(x, current->value)) current = current->left;
        else if (comp_(current->value, x)) current = current->right;
        else return iterator(current, this);
    }
    return end();
}

template<class T, class Compare, class Allocator>
template<class K>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::find(const K &x) const
{
    Node* current = root_;
    while (current)
    {
        if (comp_(x, current->value)) current = current->left;
        else if (comp_(current->value, x)) current = current->right;
        else return const_iterator(current, this);
    }
    return cend();
}

template<class T, class Compare, class Allocator>
std::pair<typename set<T, Compare, Allocator>::Node*, bool>
set<T, Compare, Allocator>::insert_node(const value_type &value)
{
    Node* y = nullptr;
    Node* x = root_;
    while (x)
    {
        y = x;
        if (comp_(value, x->value)) x = x->left;
        else if (comp_(x->value, value)) x = x->right;
        else return {x, false};
    }

    Node* z = create_node(value);
    z->parent = y;
    z->left = z->right = nullptr;
    z->is_black = false;

    if (!y)
    {
        root_ = z;
    }
    else if (comp_(z->value, y->value))
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }

    ++size_;
    if (!leftmost_ || comp_(z->value, leftmost_->value)) leftmost_ = z;
    if (!rightmost_ || comp_(rightmost_->value, z->value)) rightmost_ = z;

    fix_insert(z);
    return {z, true};
}

template<class T, class Compare, class Allocator>
std::pair<typename set<T, Compare, Allocator>::Node*, bool>
set<T, Compare, Allocator>::insert_node(value_type &&value)
{
    Node* y = nullptr;
    Node* x = root_;
    while (x)
    {
        y = x;
        if (comp_(value, x->value)) x = x->left;
        else if (comp_(x->value, value)) x = x->right;
        else return {x, false};
    }

    Node* z = create_node(std::move(value));
    z->parent = y;
    z->left = z->right = nullptr;
    z->is_black = false;

    if (!y)
    {
        root_ = z;
    }
    else if (comp_(z->value, y->value))
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }

    ++size_;
    if (!leftmost_ || comp_(z->value, leftmost_->value)) leftmost_ = z;
    if (!rightmost_ || comp_(rightmost_->value, z->value)) rightmost_ = z;

    fix_insert(z);
    return {z, true};
}

template<class T, class Compare, class Allocator>
std::pair<typename set<T, Compare, Allocator>::iterator, bool>
set<T, Compare, Allocator>::insert(const value_type &value)
{
    auto pr = insert_node(value);
    return { iterator(pr.first, this), pr.second };
}

template<class T, class Compare, class Allocator>
std::pair<typename set<T, Compare, Allocator>::iterator, bool>
set<T, Compare, Allocator>::insert(value_type &&value)
{
    auto pr = insert_node(std::move(value));
    return { iterator(pr.first, this), pr.second };
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::insert(const_iterator, const value_type &value)
{
    return insert(value).first;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::insert(const_iterator, value_type &&value)
{
    return insert(std::move(value)).first;
}

template<class T, class Compare, class Allocator>
template<class InputIt>
void set<T, Compare, Allocator>::insert(InputIt first, InputIt last)
{
    for (; first != last; ++first)
        insert(*first);
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::insert(std::initializer_list<value_type> ilist)
{
    insert(ilist.begin(), ilist.end());
}

template<class T, class Compare, class Allocator>
template<class... Args>
std::pair<typename set<T, Compare, Allocator>::iterator, bool> set<T, Compare, Allocator>::emplace(Args&&... args)
{
    value_type v(std::forward<Args>(args)...);
    return insert(std::move(v));
}

template<class T, class Compare, class Allocator>
template<class... Args>
typename set<T, Compare, Allocator>::iterator set<T, Compare, Allocator>::emplace_hint(const_iterator, Args&&... args)
{
    return emplace(std::forward<Args>(args)...).first;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::erase_node(Node* z)
{
    if (!z) return;
    Node* y = z;
    Node* x = nullptr;
    Node* x_parent = nullptr;
    bool y_original_black = y->is_black;

    if (!z->left)
    {
        x = z->right;
        x_parent = z->parent;
        transplant(z, z->right);
    }
    else if (!z->right)
    {
        x = z->left;
        x_parent = z->parent;
        transplant(z, z->left);
    }
    else
    {
        y = minimum(z->right);
        y_original_black = y->is_black;
        x = y->right;
        if (y->parent == z)
        {
            if (x) x->parent = y;
            x_parent = y;
        }
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
            x_parent = y->parent;
        }
        transplant(z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
        y->is_black = z->is_black;
    }

    if (y_original_black)
    {
        fix_erase(x, x_parent);
    }

    if (z == leftmost_)
        leftmost_ = root_ ? minimum(root_) : nullptr;
    if (z == rightmost_)
        rightmost_ = root_ ? maximum(root_) : nullptr;

    destroy_node(z);
    --size_;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::rotate_left(Node *node)
{
    Node *y = node->right;
    node->right = y->left;

    if (y->left != nullptr)
    {
        y->left->parent = node;
    }

    y->parent = node->parent;

    if (node->parent == nullptr)
    {
        root_ = y;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = y;
    }
    else
    {
        node->parent->right = y;
    }

    y->left = node;
    node->parent = y;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::rotate_right(Node *node)
{
    Node *y = node->left;
    node->left = y->right;

    if (y->right != nullptr)
    {
        y->right->parent = node;
    }

    y->parent = node->parent;

    if (node->parent == nullptr)
    {
        root_ = y;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = y;
    }
    else
    {
        node->parent->left = y;
    }

    y->right = node;
    node->parent = y;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::fix_insert(Node *node)
{
    while (node->parent && !node->parent->is_black)
    {
        if (!node->parent->parent)
            break;
        
        if (node->parent == node->parent->parent->left)
        {
            Node *y = node->parent->parent->right;

            if (y && !y->is_black)
            {
                node->parent->is_black = true;
                y->is_black = true;
                node->parent->parent->is_black = false;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {

                    node = node->parent;
                    rotate_left(node);
                }

                node->parent->is_black = true;
                node->parent->parent->is_black = false;
                rotate_right(node->parent->parent);
            }
        }
        else
        {
            Node *y = node->parent->parent->left;

            if (y && !y->is_black)
            {
                node->parent->is_black = true;
                y->is_black = true;
                node->parent->parent->is_black = false;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    node = node->parent;
                    rotate_right(node);
                }
                node->parent->is_black = true;
                node->parent->parent->is_black = false;
                rotate_left(node->parent->parent);
            }
        }
    }
    root_->is_black = true;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::fix_erase(Node *node, Node *parent)
{
    while (node != root_ && (!node || node->is_black))
    {
        if (!parent)
            break;
        
        if (node == parent->left)
        {
            Node *w = parent->right;
            
            if (!w)
                break;

            if (!w->is_black)
            {
                w->is_black = true;
                parent->is_black = false;
                rotate_left(parent);
                w = parent->right;
                if (!w) break;
            }

            if ((!w->left || w->left->is_black) &&
                (!w->right || w->right->is_black))
            {
                w->is_black = false;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!w->right || w->right->is_black)
                {
                    if (w->left) w->left->is_black = true;
                    w->is_black = false;
                    rotate_right(w);
                    w = parent->right;
                    if (!w) break;
                }

                w->is_black = parent->is_black;
                parent->is_black = true;
                if (w->right) w->right->is_black = true;
                rotate_left(parent);
                node = root_;
            }
        }
        else
        {
            Node *w = parent->left;
            
            if (!w)
                break;

            if (!w->is_black)
            {
                w->is_black = true;
                parent->is_black = false;
                rotate_right(parent);
                w = parent->left;
                if (!w) break;
            }

            if ((!w->right || w->right->is_black) &&
                (!w->left || w->left->is_black))
            {
                w->is_black = false;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!w->left || w->left->is_black)
                {
                    if (w->right) w->right->is_black = true;
                    w->is_black = false;
                    rotate_left(w);
                    w = parent->left;
                    if (!w) break;
                }
                w->is_black = parent->is_black;
                parent->is_black = true;
                if (w->left) w->left->is_black = true;
                rotate_right(parent);
                node = root_;
            }
        }
    }
    if (node)
    {
        node->is_black = true;
    }
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::minimum(Node *node)
{
    while (node->left != nullptr)
    {
        node = node->left;
    }
    return node;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::maximum(Node *node)
{
    while (node->right != nullptr)
    {
        node = node->right;
    }
    return node;
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::successor(Node *node) const
{
    if (node->right != nullptr)
    {
        return minimum(node->right);
    }

    Node *parent = node->parent;
    while (parent != nullptr && node == parent->right)
    {
        node = parent;
        parent = parent->parent;
    }
    return parent;

}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::predecessor(Node *node) const
{
    if (node->left != nullptr)
    {
        return maximum(node->left);
    }

    Node *parent = node->parent;
    while (parent != nullptr && node == parent->left)
    {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

template<class T, class Compare, class Allocator>
void set<T, Compare, Allocator>::transplant(Node *u, Node *v)
{
    if (u->parent == nullptr)
    {
        root_ = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }

    if (v != nullptr)
    {
        v->parent = u->parent;
    }
}

template<class T, class Compare, class Alloc>
bool operator==(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class T, class Compare, class Alloc>
bool operator!=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !(lhs == rhs);
}

template<class T, class Compare, class Alloc>
bool operator<(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Compare, class Alloc>
bool operator<=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !(rhs < lhs);
}

template<class T, class Compare, class Alloc>
bool operator>(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return rhs < lhs;
}

template<class T, class Compare, class Alloc>
bool operator>=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !(lhs < rhs);
}

template<class T, class Compare, class Alloc>
void swap(set<T, Compare, Alloc>& lhs, set<T, Compare, Alloc>& rhs) noexcept;

template<class T, class Compare, class Alloc>
void swap(set<T, Compare, Alloc> &lhs, set<T, Compare, Alloc> &rhs) noexcept
{
    lhs.swap(rhs);
}

template<class T, class Compare, class Alloc, class Pred>
typename set<T, Compare, Alloc>::size_type erase_if(set<T, Compare, Alloc>& c, Pred pred);

template<class T, class Compare, class Alloc, class Pred>
typename set<T, Compare, Alloc>::size_type erase_if(set<T, Compare, Alloc> &c, Pred pred)
{
    size_t old_size = c.size();
    for (auto it = c.begin(); it != c.end(); )
    {
        if (pred(*it))
        {
            it = c.erase(it);
        }
        else
        {
            ++it;
        }
    }
    return old_size - c.size();
}

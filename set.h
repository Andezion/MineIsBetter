#pragma once

#include <functional>
#include <memory>
#include <iterator>
#include <initializer_list>
#include <utility>

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
    : node_(it.node_), tree_(nullptr) {}

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
set<T, Compare, Allocator>::set(const set &other)
    : root_(nullptr),
      size_(0),
      comp_(other.comp_),
      alloc_(other.alloc_)
{
    for (const T& val : other)
    {
        insert(val);
    }
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(const set &other, const Allocator &alloc)
    : root_(nullptr),
      size_(0),
      comp_(other.comp_),
      alloc_(alloc)
{
    for (const T& val : other)
    {
        insert(val);
    }
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(set &&other) noexcept
    : root_(other.root_), leftmost_(nullptr), rightmost_(nullptr),
      size_(other.size_),
      comp_(std::move(other.comp_)),
      alloc_(std::move(other.alloc_))
{
    other.root_ = nullptr;
    other.size_ = 0;
}


template<class T, class Compare, class Allocator>
set<T, Compare, Allocator>::set(set &&other, const Allocator &alloc)
    : root_(nullptr),
      size_(0),
      comp_(std::move(other.comp_)),
      alloc_(alloc)
{
    for (const T& val : other)
    {
        insert(std::move(val));
    }

    other.clear();
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

    root_ = std::move(other.root_);

    other.size_ = 0;
    other.root_ = nullptr;

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
    return iterator(min_node(root_), this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::begin() const noexcept
{
    return const_iterator(min_node(root_), this);
}

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::const_iterator set<T, Compare, Allocator>::cbegin() const noexcept
{
    return const_iterator(min_node(root_), this);
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
    return size_;
}

template<class T, class Compare, class Alloc>
bool operator==(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs);

template<class T, class Compare, class Allocator>
typename set<T, Compare, Allocator>::Node * set<T, Compare, Allocator>::create_node(const value_type &value)
{
    Node *node = alloc_.allocate(1);
    try
    {
        std::allocator_traits<NodeAllocator>::construct(alloc_, &node->value, value);
        node->left = node->right = node->parent = nullptr;
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
        std::allocator_traits<NodeAllocator>::construct(alloc_, &node->value, std::move(value));
        node->left = node->right = node->parent = nullptr;
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

    std::allocator_traits<NodeAllocator>::destroy(alloc_, &node->value);

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

template<class T, class Compare, class Alloc>
bool operator==(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return lhs.equals(rhs);
}

template<class T, class Compare, class Alloc>
bool operator!=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs);

template<class T, class Compare, class Alloc>
bool operator!=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return !lhs.equals(rhs);
}

template<class T, class Compare, class Alloc>
bool operator<(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs);

template<class T, class Compare, class Alloc>
bool operator<(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return lhs.less(rhs);
}

template<class T, class Compare, class Alloc>
bool operator<=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs);

template<class T, class Compare, class Alloc>
bool operator<=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return lhs.less(rhs) || lhs.equals(rhs);
}

template<class T, class Compare, class Alloc>
bool operator>(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs);

template<class T, class Compare, class Alloc>
bool operator>(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return rhs.less(lhs);
}

template<class T, class Compare, class Alloc>
bool operator>=(const set<T, Compare, Alloc>& lhs, const set<T, Compare, Alloc>& rhs);

template<class T, class Compare, class Alloc>
bool operator>=(const set<T, Compare, Alloc> &lhs, const set<T, Compare, Alloc> &rhs)
{
    return rhs.less(lhs) || rhs.equals(rhs);
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

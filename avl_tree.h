#pragma once

#include <algorithm>

template<typename K, typename V>
class avl_tree 
{
    struct Node
    {
        K key;
        V val;

        Node* left = nullptr;
        Node* right = nullptr;

        int height = 1;

        Node(const K& k, const V& v): key(k), val(v) {}
    };

public:
    void insert(const K& k, const V& v)
    {
        root_ = insert_impl(root_, k, v);
    }

    V* find(const K& k)
    {
        Node* cur = root_;
        while (cur)
        {
            if (k == cur->key) 
            {
                return &cur->val;
            }
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        return nullptr;
    }

private:
    int height(Node* n) const 
    { 
        return n ? n->height : 0; 
    }

    int balance_factor(Node* n) const 
    { 
        return n ? height(n->left) - height(n->right) : 0; 
    }

    void update_height(Node* n)
    {
        if (n) 
        {
            n->height = 1 + std::max(height(n->left), height(n->right));
        }
    }

    Node* rotate_right(Node* y)
    {
        Node* x = y->left;

        y->left = x->right;
        x->right = y;

        update_height(y);
        update_height(x);

        return x;
    }

    Node* rotate_left(Node* x)
    {
        Node* y = x->right;

        x->right = y->left;
        y->left = x;

        update_height(x);
        update_height(y);

        return y;
    }

    Node* balance(Node* n)
    {
        update_height(n);

        int bf = balance_factor(n);
        if (bf > 1)
        {
            if (balance_factor(n->left) < 0)
            {
                n->left = rotate_left(n->left);
            }
            return rotate_right(n);
        }
        if (bf < -1)
        {
            if (balance_factor(n->right) > 0)
            {
                n->right = rotate_right(n->right);
            }
            return rotate_left(n);
        }
        return n;
    }

    Node* insert_impl(Node* node, const K& k, const V& v)
    {
        if (!node) 
        {
            return new Node(k, v);
        }
        if (k == node->key)
        {
            node->val = v;
            return node;
        }

        if (k < node->key) 
        {
            node->left = insert_impl(node->left, k, v);
        }
        else 
        {
            node->right = insert_impl(node->right, k, v);
        }
        return balance(node);
    }

    Node* root_ = nullptr;
};

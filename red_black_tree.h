#pragma once

#include <utility>

template<typename K, typename V>
class red_black_tree 
{
    enum Color { RED, BLACK };

    struct Node 
    {
        K key;
        V val;

        Node* left = nullptr;
        Node* right = nullptr;

        Color color = RED;

        Node(const K& k, const V& v, Color c = RED): key(k), val(v), color(c) {}
    };

public:
    void insert(const K& k, const V& v) 
    {
        root_ = insert_impl(root_, k, v);
        if (root_) root_->color = BLACK;
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
    static bool is_red(Node* n) 
    { 
        return n && n->color == RED; 
    }

    Node* rotate_left(Node* h)
    {
        Node* x = h->right;

        h->right = x->left;
        x->left = h;

        x->color = h->color;
        h->color = RED;

        return x;
    }

    Node* rotate_right(Node* h)
    {
        Node* x = h->left;

        h->left = x->right;
        x->right = h;

        x->color = h->color;
        h->color = RED;

        return x;
    }

    void flip_colors(Node* h)
    {
        h->color = (h->color == RED) ? BLACK : RED;
        if (h->left) 
        {
            h->left->color = (h->left->color == RED) ? BLACK : RED;
        }
        if (h->right) 
        {
            h->right->color = (h->right->color == RED) ? BLACK : RED;
        }
    }

    Node* insert_impl(Node* h, const K& k, const V& v)
    {
        if (!h) 
        {
            return new Node(k, v, RED);
        }

        if (k < h->key) 
        {
            h->left = insert_impl(h->left, k, v);
        }
        else if (k > h->key) 
        {
            h->right = insert_impl(h->right, k, v);
        }
        else 
        {
            h->val = v;
        }

        if (is_red(h->right) && !is_red(h->left)) 
        {
            h = rotate_left(h);
        }
        if (is_red(h->left) && is_red(h->left->left)) 
        {
            h = rotate_right(h);
        }
        if (is_red(h->left) && is_red(h->right)) 
        {
            flip_colors(h);
        }

        return h;
    }

    Node* root_ = nullptr;
};

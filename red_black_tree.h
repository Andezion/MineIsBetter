#pragma once

#include <utility>

template<typename K, typename V>
class red_black_tree 
{
    struct Node 
    {
        K key;
        V val;

        Node* left = nullptr;
        Node* right = nullptr;

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
    Node* insert_impl(Node* node, const K& k, const V& v) {
        if (!node) return new Node(k, v);
        if (k == node->key) { node->val = v; return node; }
        if (k < node->key) node->left = insert_impl(node->left, k, v);
        else node->right = insert_impl(node->right, k, v);
        return node;
    }

    Node* root_ = nullptr;
};

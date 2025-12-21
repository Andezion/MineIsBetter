#pragma once

#include <random>

template<typename K, typename V>
class treap 
{
    struct Node 
    {
        K key;
        V val;

        unsigned priority;

        Node* left = nullptr;
        Node* right = nullptr;
        
        Node(const K& k, const V& v, unsigned p): key(k), val(v), priority(p) {}
    };

public:
    treap(): rng_(std::random_device{}()) {}

    void insert(const K& k, const V& v) {
        root_ = insert_impl(root_, k, v);
    }

    V* find(const K& k) {
        Node* cur = root_;
        while (cur) {
            if (k == cur->key) return &cur->val;
            cur = (k < cur->key) ? cur->left : cur->right;
        }
        return nullptr;
    }

private:
    Node* rotate_right(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    Node* rotate_left(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    Node* insert_impl(Node* node, const K& k, const V& v) {
        if (!node) return new Node(k, v, dist_(rng_));
        if (k == node->key) { node->val = v; return node; }
        if (k < node->key) {
            node->left = insert_impl(node->left, k, v);
            if (node->left->priority > node->priority) node = rotate_right(node);
        } else {
            node->right = insert_impl(node->right, k, v);
            if (node->right->priority > node->priority) node = rotate_left(node);
        }
        return node;
    }

    Node* root_ = nullptr;
    std::mt19937 rng_;
    std::uniform_int_distribution<unsigned> dist_ {0, std::numeric_limits<unsigned>::max()};
};

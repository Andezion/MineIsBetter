#pragma once

#include <string>
#include <memory>
#include <utility>

class rope
{
    struct Node
    {
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        std::string s;
        size_t len = 0;

        explicit Node(const std::string& str) : s(str), len(str.size()) {}
        Node(std::shared_ptr<Node> l, std::shared_ptr<Node> r) : left(std::move(l)), right(std::move(r))
        {
            len = 0;
            if (left)
            {
                len += left->len;
            }
            if (right)
            {
                len += right->len;
            }
        }
    };

public:
    rope() = default;
    explicit rope(const std::string& s) : root_(std::make_shared<Node>(s)) {}

    rope& append(const rope& r)
    {
        if (!root_)
        {
            root_ = r.root_; return *this;
        }
        if (!r.root_)
        {
            return *this;
        }
        root_ = std::make_shared<Node>(root_, r.root_);
        return *this;
    }

    [[nodiscard]] size_t size() const
    {
        return root_ ? root_->len : 0;
    }

    [[nodiscard]] std::string str() const
    {
        std::string out;
        if (!root_)
        {
            return out;
        }

        out.reserve(root_->len);
        append_to_string(root_, out);

        return out;
    }

    rope operator+(const rope& other) const
    {
        rope r;
        r.root_ = std::make_shared<Node>(root_, other.root_);
        return r;
    }

    rope& operator+=(const rope& other)
    {
        return append(other);
    }

private:
    void append_to_string(const std::shared_ptr<Node>& n, std::string& out) const
    {
        if (!n)
        {
            return;
        }
        if (!n->left && !n->right)
        {
            out += n->s; return;
        }

        append_to_string(n->left, out);
        append_to_string(n->right, out);
    }

    std::shared_ptr<Node> root_;
};

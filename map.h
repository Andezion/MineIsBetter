#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <iterator>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <limits>

template<
	typename Key,
	typename T,
	typename Compare = std::less<Key>,
	typename Allocator = std::allocator<std::pair<const Key, T>>
>
class map
{
public:
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<const Key, T>;
	using key_compare = Compare;
	using allocator_type = Allocator;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = typename std::allocator_traits<Allocator>::pointer;
	using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

private:
	struct Node
	{
		value_type kv;
		Node* parent;
		Node* left;
		Node* right;

		template<typename... Args>
		explicit Node(Args&&... args)
			: kv(std::forward<Args>(args)...), parent(nullptr), left(nullptr), right(nullptr) {}
	};

	Node* root_ = nullptr;
	size_type size_ = 0;
	key_compare comp_;
	allocator_type alloc_;

public:
	struct iterator;
	struct const_iterator;

	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	map() = default;
	explicit map(const key_compare& comp, const allocator_type& alloc = allocator_type());
	map(const map& other);
	map(map&& other) noexcept;
	map(std::initializer_list<value_type> init, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type());
	~map() = default;

	map& operator=(const map& other);
	map& operator=(map&& other) noexcept;

	allocator_type get_allocator() const noexcept { return alloc_; }

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;

	bool empty() const noexcept { return size_ == 0; }
	size_type size() const noexcept { return size_; }
	size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); }

	mapped_type& operator[](const key_type& key);
	mapped_type& at(const key_type& key);
	const mapped_type& at(const key_type& key) const;

	void clear() noexcept;
	std::pair<iterator, bool> insert(const value_type& v);
	std::pair<iterator, bool> insert(value_type&& v);
	iterator insert(const_iterator hint, const value_type& v);
	template<class... Args>
	std::pair<iterator, bool> emplace(Args&&... args);
	iterator erase(const_iterator pos);
	size_type erase(const key_type& key);

	iterator find(const key_type& key);
	const_iterator find(const key_type& key) const;
	size_type count(const key_type& key) const;
	iterator lower_bound(const key_type& key);
	const_iterator lower_bound(const key_type& key) const;
	iterator upper_bound(const key_type& key);
	const_iterator upper_bound(const key_type& key) const;

	key_compare key_comp() const 
	{ 
		return comp_; 
	}

	friend bool operator==(const map& a, const map& b) 
	{ 
		return a.size_ == b.size_; 
	}
	friend bool operator!=(const map& a, const map& b) 
	{ 
		return !(a == b); 
	}
};

template<typename Key, typename T, typename Compare, typename Allocator>
struct map<Key,T,Compare,Allocator>::iterator 
{
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename map::value_type;
	using difference_type = std::ptrdiff_t;
	using pointer = typename map::pointer;
	using reference = value_type&;

	Node* node = nullptr;
	iterator() = default;

	explicit iterator(Node* n) : node(n) {}

	reference operator*() const 
	{ 
		return node->kv; 
	}
	pointer operator->() const 
	{ 
		return std::addressof(node->kv); 
	}

	iterator& operator++() 
	{
		if (!node) 
		{
			return *this;
		}

		if (node->right) 
		{
			Node* p = node->right;

			while (p->left) 
			{
				p = p->left;
			}

			node = p;
			return *this;
		}

		Node* p = node->parent;
		Node* cur = node;

		while (p && cur == p->right) 
		{ 
			cur = p; 
			p = p->parent; 
		}

		node = p;
		return *this;
	}

	iterator& operator--() 
	{
		if (!node) 
		{
			return *this; 
		}
		if (node->left) 
		{
			Node* p = node->left;

			while (p->right) 
			{
				p = p->right;
			}

			node = p;
			return *this;
		}

		Node* p = node->parent;
		Node* cur = node;

		while (p && cur == p->left) 
		{ 
			cur = p; 
			p = p->parent; 
		}

		node = p;
		return *this;
	}
	bool operator==(const iterator& o) const noexcept 
	{ 
		return node == o.node; 
	}
	bool operator!=(const iterator& o) const noexcept 
	{ 
		return node != o.node; 
	}
};

template<typename Key, typename T, typename Compare, typename Allocator>
struct map<Key,T,Compare,Allocator>::const_iterator 
{
	using iterator_category = std::bidirectional_iterator_tag;
	using value_type = typename map::value_type;
	using difference_type = std::ptrdiff_t;
	using pointer = typename map::const_pointer;
	using reference = const value_type&;

	const Node* node = nullptr;
	const_iterator() = default;

	explicit const_iterator(const Node* n) : node(n) {}
	const_iterator(const iterator& it) : node(it.node) {}

	reference operator*() const 
	{ 
		return node->kv; 
	}
	pointer operator->() const 
	{ 
		return std::addressof(node->kv); 
	}

	const_iterator& operator++() 
	{
		if (!node) 
		{
			return *this;
		}

		if (node->right) 
		{
			const Node* p = node->right;

			while (p->left) 
			{
				p = p->left;
			}

			node = p;
			return *this;
		}

		const Node* p = node->parent;
		const Node* cur = node;

		while (p && cur == p->right) 
		{ 
			cur = p; 
			p = p->parent; 
		}

		node = p;
		return *this;
	}

	const_iterator& operator--() 
	{
		if (!node) return *this;

		if (node->left) 
		{
			const Node* p = node->left;

			while (p->right) 
			{
				p = p->right;
			}

			node = p;
			return *this;
		}

		const Node* p = node->parent;
		const Node* cur = node;

		while (p && cur == p->left) 
		{ 
			cur = p; 
			p = p->parent; 
		}

		node = p;
		return *this;
	}

	bool operator==(const const_iterator& o) const noexcept 
	{ 
		return node == o.node; 
	}
	bool operator!=(const const_iterator& o) const noexcept 
	{ 
		return node != o.node; 
	}
};

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::Node*
minimum(typename map<Key,T,Compare,Allocator>::Node* n) 
{
	if (!n) 
	{
		return nullptr;
	}
	while (n->left) 
	{
		n = n->left;
	}
	return n;
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::Node*
maximum(typename map<Key,T,Compare,Allocator>::Node* n) 
{
	if (!n) 
	{
		return nullptr;
	}
	while (n->right) 
	{
		n = n->right;
	}
	return n;
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::begin() noexcept 
{ 
	return iterator(minimum(root_)); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::begin() const noexcept 
{ 
	return const_iterator(minimum(root_)); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::end() noexcept 
{ 
	return iterator(nullptr); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::end() const noexcept 
{ 
	return const_iterator(nullptr); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
void map<Key,T,Compare,Allocator>::clear() noexcept 
{
	if (!root_) 
	{
		return;
	}

	std::vector<Node*> stack;
	stack.push_back(root_);

	while (!stack.empty()) 
	{
		Node* n = stack.back(); stack.pop_back();

		if (n->left) 
		{
			stack.push_back(n->left);
		}
		if (n->right) 
		{
			stack.push_back(n->right);
		}

		delete n;
	}
	root_ = nullptr;
	size_ = 0;
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::find(const key_type& key) 
{
	Node* cur = root_;
	while (cur) 
	{
		if (!comp_(cur->kv.first, key) && !comp_(key, cur->kv.first)) 
		{
			return iterator(cur);
		}
		if (comp_(key, cur->kv.first)) 
		{
			cur = cur->left; 
		} 
		else 
		{
			cur = cur->right;
		}
	}
	return end();
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::find(const key_type& key) const 
{
	const Node* cur = root_;
	while (cur) 
	{
		if (!comp_(cur->kv.first, key) && !comp_(key, cur->kv.first)) 
		{
			return const_iterator(cur);
		}
		if (comp_(key, cur->kv.first)) 
		{
			cur = cur->left; 
		} 
		else 
		{
			cur = cur->right;
		}
	}
	return end();
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::lower_bound(const key_type& key) 
{
	Node* x = root_;
	Node* res = nullptr;

	while (x) 
	{
		if (comp_(x->kv.first, key)) 
		{
			x = x->right;
		} 
		else 
		{
			res = x;
			x = x->left;
		}
	}
	return iterator(res);
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::lower_bound(const key_type& key) const 
{
	const Node* x = root_;
	const Node* res = nullptr;

	while (x) 
	{
		if (comp_(x->kv.first, key)) 
		{
			x = x->right;
		} 
		else 
		{
			res = x;
			x = x->left;
		}
	}
	return const_iterator(res);
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::upper_bound(const key_type& key) 
{
	Node* x = root_;
	Node* res = nullptr;

	while (x) 
	{
		if (comp_(key, x->kv.first)) 
		{
			res = x;
			x = x->left;
		} 
		else 
		{
			x = x->right;
		}
	}
	return iterator(res);
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::upper_bound(const key_type& key) const 
{
	const Node* x = root_;
	const Node* res = nullptr;

	while (x) 
	{
		if (comp_(key, x->kv.first)) 
		{
			res = x;
			x = x->left;
		} 
		else 
		{
			x = x->right;
		}
	}
	return const_iterator(res);
}

template<typename Key, typename T, typename Compare, typename Allocator>
std::pair<typename map<Key,T,Compare,Allocator>::iterator, bool>
map<Key,T,Compare,Allocator>::insert(const value_type& v) 
{
	Node* parent = nullptr;
	Node* cur = root_;

	while (cur) 
	{
		parent = cur;
		if (!comp_(cur->kv.first, v.first) && !comp_(v.first, cur->kv.first)) 
		{
			return { iterator(cur), false };
		}
		if (comp_(v.first, cur->kv.first)) 
		{
			cur = cur->left; 
		}
		else 
		{
			cur = cur->right;
		}
	}

	Node* n = new Node(v);
	n->parent = parent;

	if (!parent) 
	{
		root_ = n;
	}
	else if (comp_(n->kv.first, parent->kv.first)) 
	{
		parent->left = n; 
	}
	else 
	{
		parent->right = n;
	}

	++size_;
	return 
	{ 
		iterator(n), 
		true 
	};
}

template<typename Key, typename T, typename Compare, typename Allocator>
std::pair<typename map<Key,T,Compare,Allocator>::iterator, bool>
map<Key,T,Compare,Allocator>::insert(value_type&& v) 
{
	return insert(static_cast<const value_type&>(v));
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::insert(const_iterator /*hint*/, const value_type& v) 
{
	return insert(v).first;
}

template<typename Key, typename T, typename Compare, typename Allocator>
template<class... Args>
std::pair<typename map<Key,T,Compare,Allocator>::iterator, bool>
map<Key,T,Compare,Allocator>::emplace(Args&&... args) 
{
	value_type val(std::forward<Args>(args)...);
	return insert(val);
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::mapped_type&
map<Key,T,Compare,Allocator>::operator[](const key_type& key) 
{
	auto it = lower_bound(key);
	if (it != end() && !comp_(key, it.node->kv.first) && !comp_(it.node->kv.first, key)) 
	{
		return it.node->kv.second;
	}

	auto pr = insert(value_type(key, mapped_type()));
	return pr.first.node->kv.second;
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::mapped_type&
map<Key,T,Compare,Allocator>::at(const key_type& key) 
{
	auto it = find(key);
	if (it == end()) 
	{
		throw std::out_of_range("map::at");
	}
	return it.node->kv.second;
}

template<typename Key, typename T, typename Compare, typename Allocator>
const typename map<Key,T,Compare,Allocator>::mapped_type&
map<Key,T,Compare,Allocator>::at(const key_type& key) const 
{
	auto it = find(key);
	if (it == end()) 
	{
		throw std::out_of_range("map::at");
	}
	return it.node->kv.second;
}


#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <iterator>
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <vector>
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

	void swap(map& other) noexcept;

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
	map* owner = nullptr;
	iterator() = default;

	explicit iterator(Node* n, map* o = nullptr) : node(n), owner(o) {}

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
			if (owner) { node = maximum(owner->root_); }
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
	const map* owner = nullptr;
	const_iterator() = default;

	explicit const_iterator(const Node* n, const map* o = nullptr) : node(n), owner(o) {}
	const_iterator(const iterator& it) : node(it.node), owner(it.owner) {}

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
		if (!node) {
			if (owner) { node = maximum(owner->root_); }
			return *this;
		}

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
clone_subtree(const typename map<Key,T,Compare,Allocator>::Node* src, typename map<Key,T,Compare,Allocator>::Node* parent)
{
	if (!src) 
	{
		return nullptr;
	}

	using Map = map<Key,T,Compare,Allocator>;
	Map::Node* n = new Map::Node(src->kv);

	n->parent = parent;

	n->left = clone_subtree<Key,T,Compare,Allocator>(src->left, n);
	n->right = clone_subtree<Key,T,Compare,Allocator>(src->right, n);

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
	return iterator(minimum(root_), this); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::begin() const noexcept 
{ 
	return const_iterator(minimum(root_), this); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::end() noexcept 
{ 
	return iterator(nullptr, this); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_iterator
map<Key,T,Compare,Allocator>::end() const noexcept 
{ 
	return const_iterator(nullptr, this); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::reverse_iterator
map<Key,T,Compare,Allocator>::rbegin() noexcept 
{ 
	return reverse_iterator(end()); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_reverse_iterator
map<Key,T,Compare,Allocator>::rbegin() const noexcept 
{ 
	return const_reverse_iterator(end()); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::reverse_iterator
map<Key,T,Compare,Allocator>::rend() noexcept 
{ 
	return reverse_iterator(begin()); 
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::const_reverse_iterator
map<Key,T,Compare,Allocator>::rend() const noexcept 
{ 
	return const_reverse_iterator(begin()); 
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
			return iterator(cur, this);
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
map<Key,T,Compare,Allocator>::map(const map& other)
	: root_(nullptr), size_(0), comp_(other.comp_), alloc_(other.alloc_)
{
	root_ = clone_subtree<Key,T,Compare,Allocator>(other.root_, nullptr);
	size_ = other.size_;
}

template<typename Key, typename T, typename Compare, typename Allocator>
map<Key,T,Compare,Allocator>::map(map&& other) noexcept
	: root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)), alloc_(std::move(other.alloc_))
{
	other.root_ = nullptr;
	other.size_ = 0;
}

template<typename Key, typename T, typename Compare, typename Allocator>
map<Key,T,Compare,Allocator>&
map<Key,T,Compare,Allocator>::operator=(const map& other)
{
	if (this == &other) 
	{
		return *this;
	}

	clear();

	comp_ = other.comp_;
	alloc_ = other.alloc_;

	root_ = clone_subtree<Key,T,Compare,Allocator>(other.root_, nullptr);
	size_ = other.size_;

	return *this;
}

template<typename Key, typename T, typename Compare, typename Allocator>
map<Key,T,Compare,Allocator>&
map<Key,T,Compare,Allocator>::operator=(map&& other) noexcept
{
	if (this == &other) 
	{
		return *this;
	}

	clear();

	root_ = other.root_;
	size_ = other.size_;

	comp_ = std::move(other.comp_);
	alloc_ = std::move(other.alloc_);

	other.root_ = nullptr;
	other.size_ = 0;

	return *this;
}

template<typename Key, typename T, typename Compare, typename Allocator>
void map<Key,T,Compare,Allocator>::swap(map& other) noexcept
{
	using std::swap;
	
	swap(root_, other.root_);
	swap(size_, other.size_);
	swap(comp_, other.comp_);
	swap(alloc_, other.alloc_);
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
			return const_iterator(cur, this);
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
	return iterator(res, this);
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
	return const_iterator(res, this);
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
	return iterator(res, this);
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
	return const_iterator(res, this);
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
			return { iterator(cur, this), false };
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
	return { 
		iterator(n, this), 
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

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::iterator
map<Key,T,Compare,Allocator>::erase(const_iterator pos)
{
	if (!pos.node) 
	{
		return end();
	}

	Node* z = const_cast<Node*>(pos.node);

	iterator succ(pos.node, this);
	++succ;

	auto transplant = [&](Node* u, Node* v) 
	{
		if (!u->parent) 
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

		if (v) 
		{
			v->parent = u->parent;
		}
	};

	if (!z->left) 
	{
		transplant(z, z->right);
	} 
	else if (!z->right) 
	{
		transplant(z, z->left);
	} 
	else 
	{
		Node* y = minimum(z->right);

		if (y->parent != z) 
		{
			transplant(y, y->right);
			y->right = z->right;

			if (y->right) 
			{
				y->right->parent = y;
			}
		}

		transplant(z, y);
		y->left = z->left;

		if (y->left) 
		{
			y->left->parent = y;
		}
	}

	delete z;
	--size_;
	return succ;
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::size_type
map<Key,T,Compare,Allocator>::erase(const key_type& key)
{
	auto it = find(key);

	if (it == end()) 
	{
		return 0;
	}
	erase(it);
	return 1;
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::size_type
map<Key,T,Compare,Allocator>::count(const key_type& key) const
{
	return find(key) != end() ? 1 : 0;
}


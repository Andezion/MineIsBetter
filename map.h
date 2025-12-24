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
#include <optional>

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
	enum Color { RED, BLACK };

	struct Node
	{
		value_type kv;
		Node* parent;
		Node* left;
		Node* right;
		Color color;

		template<typename... Args>
		explicit Node(Args&&... args)
			: kv(std::forward<Args>(args)...), parent(nullptr), left(nullptr), right(nullptr), color(RED) {}
	};

	Node* root_ = nullptr;
	size_type size_ = 0;
	key_compare comp_;
	allocator_type alloc_;
	
	using node_allocator_type = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node>;
	using node_alloc_traits = std::allocator_traits<node_allocator_type>;
	node_allocator_type node_alloc_;

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
	
	struct value_compare
	{
		key_compare comp;
		explicit value_compare(key_compare c = key_compare()) : comp(c) {}
		bool operator()(const value_type& a, const value_type& b) const
		{
			return comp(a.first, b.first);
		}
	};

	bool contains(const key_type& key) const;
	std::optional<value_type> extract(const key_type& key);
	void merge(map& other);

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;

	[[nodiscard]] bool empty() const noexcept
	{
		return size_ == 0;
	}
	[[nodiscard]] size_type size() const noexcept
	{
		return size_;
	}
	[[nodiscard]] static size_type max_size() noexcept
	{
		return std::numeric_limits<size_type>::max();
	}

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
	using value_type = value_type;
	using difference_type = std::ptrdiff_t;
	using pointer = pointer;
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
			if (owner)
			{
				node = maximum(owner->root_);
			}
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
	using value_type = value_type;
	using difference_type = std::ptrdiff_t;
	using pointer = const_pointer;
	using reference = const value_type&;

	const Node* node = nullptr;
	const map* owner = nullptr;
	const_iterator() = default;

	explicit const_iterator(const Node* n, const map* o = nullptr) : node(n), owner(o) {}
	explicit const_iterator(const iterator& it) : node(it.node), owner(it.owner) {}

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
		if (!node)
		{
			if (owner)
			{
				node = maximum(owner->root_);
			}
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
map_create_node(map<Key,T,Compare,Allocator>& m, const typename map<Key,T,Compare,Allocator>::value_type& v)
{
	using Map = map<Key,T,Compare,Allocator>;
	typename Map::node_allocator_type &alloc = m.node_alloc_;
	Map::node_alloc_traits::allocate(alloc, 1);

	auto* p = Map::node_alloc_traits::allocate(alloc, 1);

	try
	{
		Map::node_alloc_traits::construct(alloc, p, v);
	}
	catch(...)
	{
		Map::node_alloc_traits::deallocate(alloc, p, 1);
		throw;
	}
	return p;
}

template<typename Key, typename T, typename Compare, typename Allocator>
void
map_destroy_node(map<Key,T,Compare,Allocator>& m, typename map<Key,T,Compare,Allocator>::Node* p)
{
	if (!p) 
	{
		return;
	}

	using Map = map<Key,T,Compare,Allocator>;

	typename Map::node_allocator_type &alloc = m.node_alloc_;
	Map::node_alloc_traits::destroy(alloc, p);
	Map::node_alloc_traits::deallocate(alloc, p, 1);
}

template<typename Key, typename T, typename Compare, typename Allocator>
void left_rotate(map<Key,T,Compare,Allocator>& m, typename map<Key,T,Compare,Allocator>::Node* x)
{
	using Node = typename map<Key,T,Compare,Allocator>::Node;

	Node* y = x->right;
	x->right = y->left;

	if (y->left) 
	{
		y->left->parent = x;
	}

	y->parent = x->parent;
	if (!x->parent) 
	{
		m.root_ = y;
	}
	else if (x == x->parent->left) 
	{
		x->parent->left = y; 
	}
	else 
	{
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

template<typename Key, typename T, typename Compare, typename Allocator>
void right_rotate(map<Key,T,Compare,Allocator>& m, typename map<Key,T,Compare,Allocator>::Node* x)
{
	using Node = typename map<Key,T,Compare,Allocator>::Node;

	Node* y = x->left;
	x->left = y->right;

	if (y->right) 
	{
		y->right->parent = x;
	}

	y->parent = x->parent;

	if (!x->parent) 
	{
		m.root_ = y;
	}
	else if (x == x->parent->right) 
	{
		x->parent->right = y; 
	}
	else 
	{
		x->parent->left = y;
	}

	y->right = x;
	x->parent = y;
}

template<typename Key, typename T, typename Compare, typename Allocator>
void rb_insert_fixup(map<Key,T,Compare,Allocator>& m, typename map<Key,T,Compare,Allocator>::Node* z)
{
	using Node = typename map<Key,T,Compare,Allocator>::Node;
	while (z->parent && z->parent->color == map<Key,T,Compare,Allocator>::RED) 
	{
		Node* gp = z->parent->parent;

		if (z->parent == gp->left) 
		{
			Node* y = gp->right;

			if (y && y->color == map<Key,T,Compare,Allocator>::RED) 
			{
				z->parent->color = map<Key,T,Compare,Allocator>::BLACK;
				y->color = map<Key,T,Compare,Allocator>::BLACK;
				gp->color = map<Key,T,Compare,Allocator>::RED;
				z = gp;
			} 
			else 
			{
				if (z == z->parent->right) 
				{ 
					z = z->parent; 
					left_rotate(m, z); 
				}

				z->parent->color = map<Key,T,Compare,Allocator>::BLACK;
				gp->color = map<Key,T,Compare,Allocator>::RED;

				right_rotate(m, gp);
			}
		} 
		else 
		{
			Node* y = gp->left;

			if (y && y->color == map<Key,T,Compare,Allocator>::RED) 
			{
				z->parent->color = map<Key,T,Compare,Allocator>::BLACK;
				y->color = map<Key,T,Compare,Allocator>::BLACK;
				gp->color = map<Key,T,Compare,Allocator>::RED;

				z = gp;
			} 
			else 
			{
				if (z == z->parent->left) 
				{ 
					z = z->parent; 
					right_rotate(m, z); 
				}

				z->parent->color = map<Key,T,Compare,Allocator>::BLACK;
				gp->color = map<Key,T,Compare,Allocator>::RED;

				left_rotate(m, gp);
			}
		}
	}
	if (m.root_) 
	{
		m.root_->color = map<Key,T,Compare,Allocator>::BLACK;
	}
}

template<typename Key, typename T, typename Compare, typename Allocator>
void transplant(map<Key,T,Compare,Allocator>& m, typename map<Key,T,Compare,Allocator>::Node* u, typename map<Key,T,Compare,Allocator>::Node* v)
{
	if (!u->parent) 
	{
		m.root_ = v;
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
}

template<typename Key, typename T, typename Compare, typename Allocator>
void rb_delete_fixup(map<Key,T,Compare,Allocator>& m, typename map<Key,T,Compare,Allocator>::Node* x, typename map<Key,T,Compare,Allocator>::Node* x_parent)
{
	using Node = typename map<Key,T,Compare,Allocator>::Node;
	while ((x != m.root_) && (!x || x->color == map<Key,T,Compare,Allocator>::BLACK)) 
	{
		if (x_parent && x == x_parent->left) 
		{
			Node* w = x_parent->right;
			if (w && w->color == map<Key,T,Compare,Allocator>::RED) 
			{
				w->color = map<Key,T,Compare,Allocator>::BLACK;
				x_parent->color = map<Key,T,Compare,Allocator>::RED;
				left_rotate(m, x_parent);

				w = x_parent->right;
			}
			if ((!w->left || w->left->color == map<Key,T,Compare,Allocator>::BLACK) && 
			(!w->right || w->right->color == map<Key,T,Compare,Allocator>::BLACK)) 
			{
				if (w) 
				{
					w->color = map<Key,T,Compare,Allocator>::RED;
				}
				x = x_parent;
				x_parent = x->parent;
			} 
			else 
			{
				if (!w->right || w->right->color == map<Key,T,Compare,Allocator>::BLACK) 
				{
					if (w->left) 
					{
						w->left->color = map<Key,T,Compare,Allocator>::BLACK;
					}
					if (w) 
					{
						w->color = map<Key,T,Compare,Allocator>::RED;
					}

					right_rotate(m, w);
					w = x_parent->right;
				}

				if (w) 
				{
					w->color = x_parent->color;
				}

				x_parent->color = map<Key,T,Compare,Allocator>::BLACK;
				if (w && w->right) 
				{
					w->right->color = map<Key,T,Compare,Allocator>::BLACK;
				}
				left_rotate(m, x_parent);
				x = m.root_;
				break;
			}
		} 
		else 
		{
			Node* w = x_parent ? x_parent->left : nullptr;

			if (w && w->color == map<Key,T,Compare,Allocator>::RED) 
			{
				w->color = map<Key,T,Compare,Allocator>::BLACK;
				if (x_parent) 
				{
					x_parent->color = map<Key,T,Compare,Allocator>::RED;
				}
				if (x_parent) 
				{
					right_rotate(m, x_parent);
				}
				w = x_parent ? x_parent->left : nullptr;
			}
			if ((!w->left || w->left->color == map<Key,T,Compare,Allocator>::BLACK) 
			&& (!w->right || w->right->color == map<Key,T,Compare,Allocator>::BLACK)) 
			{
				if (w) 
				{
					w->color = map<Key,T,Compare,Allocator>::RED;
				}

				x = x_parent;
				x_parent = x ? x->parent : nullptr;
			} 
			else 
			{
				if (!w->left || w->left->color == map<Key,T,Compare,Allocator>::BLACK) 
				{
					if (w->right) 
					{
						w->right->color = map<Key,T,Compare,Allocator>::BLACK;
					}
					if (w) 
					{
						w->color = map<Key,T,Compare,Allocator>::RED;
					}

					left_rotate(m, w);
					w = x_parent ? x_parent->left : nullptr;
				}

				if (w) 
				{
					w->color = x_parent ? x_parent->color : map<Key,T,Compare,Allocator>::BLACK;
				}

				if (x_parent) 
				{
					x_parent->color = map<Key,T,Compare,Allocator>::BLACK;
				}
				if (w && w->left) 
				{
					w->left->color = map<Key,T,Compare,Allocator>::BLACK;
				}
				if (x_parent) 
				{
					right_rotate(m, x_parent);
				}
				x = m.root_;
				break;
			}
		}
	}
	if (x) 
	{
		x->color = map<Key,T,Compare,Allocator>::BLACK;
	}
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
	auto* n = new typename Map::Node(src->kv);

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
map<Key, T, Compare, Allocator>::map(const key_compare &comp, const allocator_type &alloc)
{
	root_ = nullptr;
	size_ = 0;
	comp_ = comp;
	alloc_ = alloc;
}

template<typename Key, typename T, typename Compare, typename Allocator>
map<Key,T,Compare,Allocator>::map(const map& other)
	: root_(nullptr), comp_(other.comp_), alloc_(other.alloc_)
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
map<Key, T, Compare, Allocator>::map(std::initializer_list<value_type> init, const key_compare &comp,
	const allocator_type &alloc)
{
	root_ = nullptr;
	size_ = 0;
	comp_ = comp;
	alloc_ = alloc;

	for (const auto &v : init) 
	{
		insert(v);
	}
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
			return
			{
				iterator(cur, this),
				false
			};
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

	Node* n = map_create_node<Key,T,Compare,Allocator>(*this, v);

	n->parent = parent;
	n->left = n->right = nullptr;
	n->color = RED;

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
	rb_insert_fixup<Key,T,Compare,Allocator>(*this, n);
	return
	{
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

	auto transplant_local = [&](Node* u, Node* v)
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

	Node* y = z;
	auto y_original_color = y->color;

	Node* x = nullptr;
	Node* x_parent = nullptr;

	if (!z->left)
	{
		x = z->right;
		x_parent = z->parent;
		transplant_local(z, z->right);
	}
	else if (!z->right)
	{
		x = z->left;
		x_parent = z->parent;
		transplant_local(z, z->left);
	}
	else
	{
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;

		if (y->parent == z)
		{
			x_parent = y;
			if (x)
			{
				x->parent = y;
			}
		}
		else
		{
			transplant_local(y, y->right);
			y->right = z->right;

			if (y->right)
			{
				y->right->parent = y;
			}
			x_parent = y->parent;
		}

		transplant_local(z, y);
		y->left = z->left;

		if (y->left)
		{
			y->left->parent = y;
		}
		y->color = z->color;
	}

	map_destroy_node<Key,T,Compare,Allocator>(*this, z);
	--size_;

	if (y_original_color == BLACK)
	{
		rb_delete_fixup<Key,T,Compare,Allocator>(*this, x, x_parent);
	}
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
bool map<Key,T,Compare,Allocator>::contains(const key_type& key) const
{
	return find(key) != end();
}

template<typename Key, typename T, typename Compare, typename Allocator>
std::optional<typename map<Key,T,Compare,Allocator>::value_type>
map<Key,T,Compare,Allocator>::extract(const key_type& key)
{
	auto it = find(key);

	if (it == end())
	{
		return std::nullopt;
	}

	value_type val = std::move(it.node->kv);
	erase(it);
	return val;
}

template<typename Key, typename T, typename Compare, typename Allocator>
void map<Key,T,Compare,Allocator>::merge(map& other)
{
	while (other.root_)
	{
		Node* n = minimum(other.root_);
		if (!n)
		{
			break;
		}

		auto kv = other.extract(n->kv.first);
		if (!kv)
		{
			break;
		}
		insert(std::move(*kv));
	}
}

template<typename Key, typename T, typename Compare, typename Allocator>
typename map<Key,T,Compare,Allocator>::size_type
map<Key,T,Compare,Allocator>::count(const key_type& key) const
{
	return find(key) != end() ? 1 : 0;
}


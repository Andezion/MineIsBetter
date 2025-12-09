#pragma once

#include <memory>
#include <functional>
#include <utility>
#include <iterator>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

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

	key_compare key_comp() const { return comp_; }

	friend bool operator==(const map& a, const map& b) { return a.size_ == b.size_; }
	friend bool operator!=(const map& a, const map& b) { return !(a == b); }
};


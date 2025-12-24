#pragma once

#include <unordered_set>
#include <vector>

template<typename T>
class sparse_set {
public:
	bool insert(const T& v) { return data_.insert(v).second; }
	bool contains(const T& v) const { return data_.count(v) != 0; }
	bool erase(const T& v) { return data_.erase(v) != 0; }
	void clear() noexcept { data_.clear(); }
	size_t size() const noexcept { return data_.size(); }
	std::vector<T> values() const { return std::vector<T>(data_.begin(), data_.end()); }

private:
	std::unordered_set<T> data_;
};

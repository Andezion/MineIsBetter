#pragma once
#include <list>
#include <unordered_map>
#include <utility>

template<typename K, typename V>
class lru_cache {
public:
    explicit lru_cache(size_t capacity): capacity_(capacity) {}
    void put(const K& k, const V& v) {
        auto it = map_.find(k);
        if (it != map_.end()) {
            it->second->second = v;
            touch(it);
            return;
        }
        items_.emplace_front(k,v);
        map_[k] = items_.begin();
        if (map_.size() > capacity_) {
            map_.erase(items_.back().first);
            items_.pop_back();
        }
    }
    bool get(const K& k, V& out) {
        auto it = map_.find(k);
        if (it==map_.end()) return false;
        out = it->second->second;
        touch(it);
        return true;
    }
private:
    void touch(typename std::unordered_map<K, typename std::list<std::pair<K,V>>::iterator>::iterator it) {
        items_.splice(items_.begin(), items_, it->second);
        it->second = items_.begin();
    }
    size_t capacity_;
    std::list<std::pair<K,V>> items_;
    std::unordered_map<K, typename std::list<std::pair<K,V>>::iterator> map_;
};

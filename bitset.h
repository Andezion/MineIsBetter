#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>

class bitset {
public:
    explicit bitset(std::size_t n): bits_((n + 63)/64) {}
    void set(std::size_t i) { bits_[i/64] |= (uint64_t(1) << (i%64)); }
    void reset(std::size_t i) { bits_[i/64] &= ~(uint64_t(1) << (i%64)); }
    bool test(std::size_t i) const { return bits_[i/64] & (uint64_t(1) << (i%64)); }
private:
    std::vector<uint64_t> bits_;
};

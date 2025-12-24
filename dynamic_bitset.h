#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>

class dynamic_bitset
{
public:
    dynamic_bitset() = default;
    explicit dynamic_bitset(const std::size_t n): bits_((n + 63) / 64) {}

    void resize(const std::size_t n)
    {
        bits_.resize((n + 63) / 64);
    }
    void set(const std::size_t i)
    {
        bits_[i / 64] |= static_cast<uint64_t>(1) << i % 64;
    }
    void reset(const std::size_t i)
    {
        bits_[i / 64] &= ~(static_cast<uint64_t>(1) << i % 64);
    }
    [[nodiscard]] bool test(const std::size_t i) const
    {
        return bits_[i / 64] & static_cast<uint64_t>(1) << i % 64;
    }
private:
    std::vector<uint64_t> bits_;
};

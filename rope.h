#pragma once
#include <string>
#include <memory>

class rope {
public:
    rope() = default;
    rope(const std::string& s): data_(s) {}
    rope& append(const rope& r) { data_ += r.data_; return *this; }
    std::string str() const { return data_; }
private:
    std::string data_;
};

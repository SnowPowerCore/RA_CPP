#pragma once

#include <cstddef>

class ByteArray final
{
public:
    explicit ByteArray(size_t size, std::byte byte = static_cast<std::byte>(0));
    ByteArray(const ByteArray& other);
    ByteArray(ByteArray&& other);
    ~ByteArray();

private:
    size_t size_;
    std::byte* data_;
};

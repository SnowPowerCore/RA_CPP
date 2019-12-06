#include <algorithm>
#include <cstring>
#include <iostream>
#include <thread>

#include "ByteArray.h"

using namespace std::chrono_literals;

ByteArray::ByteArray(size_t size, std::byte byte)
    : size_(size)
    , data_(new std::byte[size_])
{
    // NOTE: Заполняем весь массив указанным байтом.
    std::fill(data_, data_ + size_, byte);

    std::cout << "Created " << this << "\n";
}

ByteArray::ByteArray(const ByteArray& other)
    : size_(other.size_)
    , data_(new std::byte[size_])
{
    // NOTE: Копируем массив.
    std::memcpy(data_, other.data_, size_);

    std::this_thread::sleep_for(5s);

    std::cout << "Copied to " << this << "\n";
}

ByteArray::ByteArray(ByteArray&& other)
    : size_(other.size_)
    , data_(other.data_)
{
    other.size_ = 0;
    other.data_ = nullptr;

    std::cout << "Moved to " << this << "\n";
}

ByteArray::~ByteArray()
{
    delete[] data_;
    size_ = 0;

    std::cout << "Removed " << this << "\n";
}

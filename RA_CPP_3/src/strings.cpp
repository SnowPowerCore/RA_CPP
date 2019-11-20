#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

struct CaseInsensitiveTraits final : std::char_traits<char> {
    static bool lt(char lhs, char rhs)
    {
        return std::tolower(lhs) < std::tolower(rhs);
    }

    static bool eq(char lhs, char rhs)
    {
        return std::tolower(lhs) == std::tolower(rhs);
    }

    static int compare(const char* lhs, const char* rhs, size_t length)
    {
        for (size_t i = 0; i < length; ++i) {
            if (lt(lhs[i], rhs[i])) return -1;
            if (lt(rhs[i], lhs[i])) return 1;
        }

        return 0;
    }
};

template<typename T>
struct Allocator {
    using value_type = T;

    Allocator() = default;

    template<typename U>
    Allocator(const Allocator<U>&) {}

    T* allocate(size_t size)
    {
        std::cout << "Allocated " << sizeof(T) * size << " bytes" << "\n";
        return new T[size];
    }

    void deallocate(T* ptr, size_t size) noexcept
    {
        std::cout << "Deallocated " << sizeof(T) * size << " bytes" << "\n";
        delete[] ptr;
    }
};

namespace
{
    [[nodiscard]] std::pair<char*, size_t> allocate_array()
    {
        constexpr size_t size = 5;

        char* array = new char[500];
        std::memcpy(array, "A lot of characters and random bytes", 500);

        array[0] = 'H';
        array[1] = 'e';
        array[2] = 'l';
        array[3] = 'l';
        array[4] = 'o';

        return {array, size};
    }
}

using CaseInsensitiveString = std::basic_string<char, CaseInsensitiveTraits>;
using String = std::basic_string<char, std::char_traits<char>, Allocator<char>>;

int main()
{
    CaseInsensitiveString a = "Hello";
    CaseInsensitiveString b = "HELLO";
    assert(a == b);

    const auto [array, size] = allocate_array();

    const String string(array);
    const String s1 = string.substr(1, 1024);
    const String s3 = string.substr(10, 200);

    std::cout << array << "\n";
    std::cout << std::string(array) << "\n";
    std::cout << std::string(array, size) << "\n";
    std::cout << std::string_view(array, size) << "\n";

    return 0;
}

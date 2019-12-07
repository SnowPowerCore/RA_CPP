#include <array>
#include <cassert>
#include <cctype>
#include <cstring>
#include <iostream>
#include <string>
#include <string_view>

/**
 * @struct CaseInsensitiveTraits
 * @brief Свойства символов регистронезависимых строк.
 */
struct CaseInsensitiveTraits final : std::char_traits<char>
{
    /**
     * @brief Определяет порядок сравнения символов на больше-меньше.
     */
    static bool lt(char lhs, char rhs)
    {
        return std::tolower(lhs) < std::tolower(rhs);
    }

    /**
     * @brief Определяет порядок сравнения символов на равенство.
     */
    static bool eq(char lhs, char rhs)
    {
        return std::tolower(lhs) == std::tolower(rhs);
    }

    /**
     * @brief Лексикографическое сравнение последовательностей символов.
     */
    static int compare(const char* lhs, const char* rhs, size_t length)
    {
        for (size_t i = 0; i < length; ++i) {
            if (lt(lhs[i], rhs[i])) return -1;
            if (lt(rhs[i], lhs[i])) return 1;
        }

        return 0;
    }
};

/**
 * @struct DebugAllocator
 * @brief Обёртка над аллокатором с выводом информации о выделение и освобождении памяти.
 * @warning Только для демонстрации. Не используйте в реальном коде.
 */
template<typename T, typename Allocator = std::allocator<T>>
struct DebugAllocator
{
    // NOTE: Пробрасываем основные свойства базового аллокатора в обёртку.
    using value_type = typename std::allocator_traits<Allocator>::value_type;
    using pointer = typename std::allocator_traits<Allocator>::pointer;
    using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
    using size_type = typename std::allocator_traits<Allocator>::size_type;
    using difference_type = typename std::allocator_traits<Allocator>::difference_type;

    // NOTE: Необходимо для получения аллокатора для типа, отличного от T.
    template<typename>
    struct rebind final
    {
        using other = DebugAllocator;
    };

    [[nodiscard]] T* allocate(size_t size)
    {
        std::cout << "Allocated " << sizeof(T) * size << " bytes" << "\n";
        return Allocator().allocate(size);
    }

    void deallocate(T* ptr, size_t size) noexcept
    {
        std::cout << "Deallocated " << sizeof(T) * size << " bytes" << "\n";
        return Allocator().deallocate(ptr, size);
    }
};

// NOTE: Предполагаем, что аллокаторы для разных типов эквивалентны.
template<typename U, typename V>
constexpr bool operator== (const DebugAllocator<U>&, const DebugAllocator<V>&) noexcept
{
    return true ;
}

namespace
{
    // NOTE: Выделяем массив символов (не строку!) на куче.
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

        return { array, size };
    }
}

/**
 * @typedef CaseInsensitiveString
 * @brief Регистронезависимая строка
 */
using CaseInsensitiveString = std::basic_string<char, CaseInsensitiveTraits>;

/**
 * @typedef String
 * @brief Строка с отладочным аллокатором.
 */
using String = std::basic_string<char, std::char_traits<char>, DebugAllocator<char>>;

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

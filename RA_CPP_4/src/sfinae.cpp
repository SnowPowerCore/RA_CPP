#include <algorithm>
#include <array>
#include <iostream>
#include <list>
#include <set>
#include <vector>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

#define EXISTS(type, expression) std::enable_if_t<std::is_same_v<type, decltype(expression)>>

namespace TypeTraits
{
    template<typename T, typename = void>
    struct IsLookupContainer : std::false_type {};

    template<typename T>
    struct IsLookupContainer<
        T, std::void_t<
            EXISTS(typename T::iterator, std::begin(std::declval<T>())),
            EXISTS(typename T::iterator, std::end(std::declval<T>())),
            EXISTS(typename T::iterator, std::declval<T>().find(typename T::value_type()))
        >
    > : std::true_type {};
}

namespace
{
    template<typename Container, typename T, REQUIRES(std::is_same_v<typename Container::value_type, T>)>
    bool constains(const Container& container, const T& value)
    {
        if constexpr (TypeTraits::IsLookupContainer<Container>::value) {
            return container.find(value) != std::end(container);
        } else {
            return std::find(std::begin(container), std::end(container), value) != std::end(container);
        }
    }
}

int main()
{
    const std::vector vector = {1, 2, 3, 4, 5};
    const std::list list = {1, 2, 3, 4, 5};
    const std::set set = {1, 2, 3, 4, 5};
    const std::array array = {1, 2, 3, 4, 5};
    const int carray[] = {1, 2, 3, 4, 5};

    std::cout << std::boolalpha << constains(set, 3) << "\n";

    return 0;
}

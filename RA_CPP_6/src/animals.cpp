#include <iostream>
#include <variant>
#include <vector>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

namespace TypeTraits
{
    template<typename T, typename Variant>
    struct ContainsType : std::false_type {};

    template<typename T, typename... Ts>
    struct ContainsType<T, std::variant<Ts...>> : std::bool_constant<(std::is_same_v<T, Ts> || ...)> {};
}

struct Cat final
{
    void voice() const
    {
        std::cout << "Meow!" << "\n";
    }
};

struct Dog final
{
    void voice() const
    {
        std::cout << "Woof!" << "\n";
    }
};

struct Human final
{
    void voice() const
    {
        std::cout << "I'm not an animal!" << "\n";
    }
};

using Animal = std::variant<Cat, Dog>;

struct Voice final
{
    template<typename T, REQUIRES(TypeTraits::ContainsType<T, Animal>::value)>
    void operator()(const T& animal) const
    {
        animal.voice();
    }
};

int main()
{
    const std::vector<Animal> animals = { Cat(), Dog() };

    for (const Animal& animal : animals) {
        std::visit(Voice(), animal);
    }

//     std::visit(Voice(), std::variant<Human>());

    return 0;
}

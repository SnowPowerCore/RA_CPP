#include <iostream>
#include <variant>
#include <vector>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

// NOTE: Статический полиморфизм, основанный на Σ-типе.
// Сравните с реализациями, из 2-го и 4-го занятий.

namespace TypeTraits
{
    /**
     * @struct ContainsType
     * @brief Свойство типа, показывающее, содержится ли указанный тип частью суммы.
     * @tparam T исследуемый тип
     * @tparam SigmaType Σ-тип (тип-сумма)
     */
    template<typename T, typename SigmaType>
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

// NOTE: Используем std::variant для определения Σ-типа "Животное".
using Animal = std::variant<Cat, Dog>;

struct Voice final
{
    // NOTE: Отображаем применение посетителя на вызовы методов полиморфных объектов.
    template<typename T, REQUIRES(TypeTraits::ContainsType<T, Animal>::value)>
    void operator()(const T& animal) const
    {
        animal.voice();
    }
};

int main()
{
    // NOTE: Общий тип позволяет использовать контейнер элементов.
    const std::vector<Animal> animals = { Cat(), Dog() };

    for (const Animal& animal : animals) {
        std::visit(Voice(), animal);
    }

    // NOTE: Человек - не животное! Такой вызов не пройдёт.
    // std::visit(Voice(), std::variant<Human>());

    return 0;
}

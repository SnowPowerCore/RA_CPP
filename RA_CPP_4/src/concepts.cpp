#include <iostream>

// NOTE: Статический полиморфизм, основанный на концептах.
// Сравните с реализацией, основанной на CRTP.

// NOTE: Определяем концепт "Животное".
template<typename T>
concept bool Animal()
{
    return requires(T animal)
    {
        { animal.name() } -> std::string;
        { animal.voice() } -> void
    };
};

struct Cat
{
    constexpr std::string_view name() const
    {
        return "Cat";
    }

    void voice()
    {
        std::cout << "Meow!" << "\n";
    }
};

struct Dog
{
    constexpr std::string_view name() const
    {
        return "Dog";
    }

    void voice() const
    {
        std::cout << "Woof!" << "\n";
    }
};

struct Tree
{
    constexpr std::string_view name() const
    {
        return "Tree";
    }
};

namespace
{
    template<Animal... Ts> // NOTE: Функция применима только к объектам классов, удовлетворяющих концепту Animal.
    void voiceAll(Ts&& ... animal)
    {
        (animal.voice(), ...);
    }
}

int main()
{
    voiceAll(Cat(), Dog());

    return 0;
}

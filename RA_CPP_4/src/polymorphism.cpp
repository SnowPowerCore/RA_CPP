#include <iostream>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

// NOTE: Статический полиморфизм, основанный на CRTP.

template<typename T>
struct Animal
{
    void voice()
    {
        static_cast<T*>(this)->voice();
    }
};

struct Cat final : Animal<Cat>
{
    void voice()
    {
        std::cout << "Meow!" << "\n";
    }
};

struct Dog final : Animal<Dog>
{
    void voice()
    {
        std::cout << "Woof!" << "\n";
    }
};

namespace
{
    // NOTE: Определяем множество полиморфтных типов (Animals).
    template<typename T>
    constexpr bool IsAnimal = std::is_base_of_v<Animal<T>, T>;

    // NOTE: Обрабатываем "коллекцию" полиморфных объектов как пакет параметров шалбонной функции.
    template<typename... Ts, REQUIRES((IsAnimal<Ts> && ...))>
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

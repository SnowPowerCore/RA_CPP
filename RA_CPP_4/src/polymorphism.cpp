#include <iostream>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

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
    template<typename T>
    constexpr bool IsAnimal = std::is_base_of_v<Animal<T>, T>;

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

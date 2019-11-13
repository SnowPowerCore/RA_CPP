#include <iostream>
#include <vector>

struct Animal {
    virtual ~Animal() = default;
    virtual void voice() = 0;
};

struct Cat final : Animal {
    void voice() override
    {
        std::cout << "Meow!" << "\n";
    }
};

struct Dog final : Animal {
    void voice() override
    {
        std::cout << "Woof!" << "\n";
    }
};

int main()
{
    const std::vector<Animal*> animals = { new Cat, new Dog };

    for (Animal* animal : animals) {
        animal->voice();
    }
}

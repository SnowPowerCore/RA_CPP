#include <array>
#include <iostream>
#include <sstream>

using namespace std::literals;

/**
 * @class Output
 * @brief Эмулятор потока вывода со встроенным форматированием.
 */
class Output final
{
public:
    ~Output()
    {
        std::cout << "\n"; // NOTE: Пишем в stdout символ переноса строки при удалении объекта.
    }

    template<typename T>
    Output& operator<<(T&& value)
    {
        std::cout << std::forward<T>(value) << " "; // NOTE: Дополняем пробелом каждое введённое значение.
        return *this;
    }
};

namespace
{
    template<typename... Ts>
    auto sum(Ts&& ... args)
    {
        return (args + ...);
    }
}

/**
 * @class Default
 * @brief Обёртка над указанным типов, реализующая конструирование по умолчанию.
 */
template<typename T, typename... Ts>
struct Default : T
{
    Default()
        : T(std::forward<Ts>(Ts())...) // NOTE: Конструирует объект базового класса параметрами по умолчанию одного из конструкторов.
    {
        // NOTE: Проверяем наличие такого конструктора у класса T.
        static_assert(std::is_constructible_v<T, Ts...>);

        // NOTE: Проверяем, что объект каждого из параметров Ts... может быть сконструирован по умолчанию..
        static_assert((std::is_default_constructible_v<Ts> && ...), "Some of args are not default constructible");
    }
};

struct User
{
    int id;
    std::string login;
    std::string password;
    bool isRegistred;

    User(int id, std::string_view login, std::string_view password, bool isRegistred)
        : id(id)
        , login(login)
        , password(password)
        , isRegistred(isRegistred)
    {}
};

int main()
{
    Output() << sum(1, 2) << sum("1"s, "2"s, "3"s) << sum(1, 2, 3, 4) << sum(1);

    // NOTE: Отустсвие конструктора по умолчанию не позволяет выделить массив пользователей.
    // std::array<User, 10> users;

    // NOTE: Обёртка решает эту проблему.
    std::array<Default<User, int, std::string, std::string, bool>, 10> users;

    return 0;
}

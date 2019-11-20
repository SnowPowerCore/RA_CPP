#include <array>
#include <iostream>
#include <sstream>

using namespace std::literals;

class Output final
{
public:
    ~Output()
    {
        std::cout << "\n";
    }

    template<typename T>
    Output& operator<<(T&& value)
    {
        std::cout << std::forward<T>(value) << " ";
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

template<typename T, typename... Ts>
struct Default : T
{
    Default()
        : T(std::forward<Ts>(Ts())...)
    {
        static_assert(std::is_constructible_v<T, Ts...>);
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

    // std::array<User, 10> users;
    std::array<Default<User, int, std::string, std::string, bool>, 10> users;

    return 0;
}

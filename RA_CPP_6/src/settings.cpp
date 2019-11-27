#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

using namespace std::literals;

struct User
{
    std::string login;
    std::string email;

    User(std::string_view login, std::string_view email)
        : login(login)
        , email(email)
    {}
};

using Property = std::variant<bool, int, std::string, User>;

template<typename... Ts>
struct Editor : Ts...
{
    Editor(Ts...) = delete;
    using Ts::operator()...;
};

namespace
{
    template<typename... Ts>
    auto editor(Ts&&... args)
    {
        return Editor { args... };
    }
}

int main()
{
    static const std::unordered_map<std::string, Property> settings = {
        { "alice", User("alice", "alice@company.com") },
        { "bob", User("bob_456", "bob@company.com") },
        { "lang", "C++"s },
        { "number", 42 },
        { "is_ready", false },
        { "is_registred", false },
    };

    for (auto&& [key, value] : settings) {
        std::cout << key << ": ";

        std::visit(editor(
            [](bool value) {
                std::cout << std::boolalpha << value << " [CheckBox]" << "\n";
            },
            [](int value) {
                std::cout << value << " [SpinBox]" << "\n";
            },
            [](const std::string& value) {
                std::cout << value << " [LineEdit]" << "\n";
            },
            [](const User& value) {
                std::cout << value.login << " " << value.email << " [UserForm]" << "\n";
            }
        ), value);
    }

    return 0;
}

#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

// NOTE: Макет редактора свойств, основанного на std::variant.

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

// NOTE: Определяем тип "Свойство" как сумму зарегистрировнных в редакторе типов значений.
using Property = std::variant<bool, int, std::string, User>;

// NOTE: Определяем редактор как класс-посетитель путём наследования обработчиков отдельных типов свойств.
// Каждый редактор - есть Callable, применимый к свойству).
template<typename... Ts>
struct Editor : Ts...
{
    using Ts::operator()...;
};

// NOTE: Определяем правила вывода шаблонных параметров редактора.
template<typename... Ts> Editor(Ts...) -> Editor<Ts...>;

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

        // NOTE: Строим и применяем редактор свойств.
        std::visit(
            Editor {
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
            }, value
        );
    }

    return 0;
}

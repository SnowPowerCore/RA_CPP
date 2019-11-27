#include <any>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

using namespace std::literals;

class Storage final
{
public:
    template<typename T>
    std::optional<T> getValue(const std::string& key) const
    {
        const auto it = storage_.find(key);
        const std::any value = (it == storage_.cend()) ? std::any() : it->second;

        return (value.has_value() && value.type() == typeid(T))
            ? std::make_optional(std::any_cast<T>(value))
            : std::nullopt;
    }

    template<typename T>
    void setValue(std::string_view key, T&& value)
    {
        storage_.emplace(key, std::forward<T>(value));
    }

private:
    std::unordered_map<std::string, std::any> storage_;
};

struct User
{
    std::string login;
    std::string email;

    User(std::string_view login, std::string_view email)
        : login(login)
        , email(email)
    {}
};

namespace
{
    template<typename T>
    std::ostream& operator<<(std::ostream& out, const std::optional<T>& optional)
    {
        if (optional.has_value()) {
            out << optional.value();
        } else {
            out << "[ NONE ]";
        }

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const User& user)
    {
        return out << "User: " << user.login << ", " << user.email;
    }
}

int main()
{
    Storage storage;
    storage.setValue("name", "Alice"s);
    storage.setValue("age", 32);
    storage.setValue("user", User("alice", "alice@company.com"));

    std::cout << storage.getValue<std::string>("name")
              << " " << storage.getValue<int>("age")
              << " " << storage.getValue<User>("user") << "\n";

    std::cout << storage.getValue<User>("name")
              << " " << storage.getValue<long>("age") << "\n";

    return 0;
}

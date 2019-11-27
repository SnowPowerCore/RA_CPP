#include <iostream>
#include <optional>
#include <string>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

namespace Pointers
{
    struct City {
        std::string country;
        std::string name;
    };

    struct Address {
        std::string street;
        int building;
    };

    struct Location {
        City* city = nullptr;
        Address* address = nullptr;
    };

    struct Person {
        std::string firstName;
        std::string lastName;
        int age;
        Location* location = nullptr;
    };

    struct User {
        Person* person = nullptr;
        std::string emails;
    };

    void printCity(const User& user)
    {
        if (user.person) {
            if (user.person->location) {
                if (user.person->location->city) {
                    std::cout << user.person->location->city->name;
                } else {
                    std::cout << "No city";
                }
            } else {
                std::cout << "No location";
            }
        } else {
            std::cout << "No person";
        }

        std::cout << "\n";
    }
}

namespace
{
    template<typename T, typename Callable, typename... Ts, REQUIRES(std::is_invocable_v<Callable, T, Ts...>)>
    constexpr auto mbind(const std::optional<T>& optional, Callable&& callable, Ts&&... args)
    {
        return optional.has_value()
            ? std::make_optional(callable(optional.value(), std::forward<Ts>(args)...))
            : std::nullopt;
    }
}

namespace Optionals
{
    struct City {
        std::string country;
        std::string name;
    };

    struct Address {
        std::string street;
        int building;
    };

    struct Location {
        std::optional<City> city;
        std::optional<Address> address;
    };

    struct Person {
        std::string firstName;
        std::string lastName;
        int age;
        std::optional<Location> location;
    };

    struct User {
        std::optional<Person> person;
        std::string emails;
    };

    void printCity(const User& user)
    {
        std::cout << mbind(user.person, [](const Person& person){
            return mbind(person.location, [](const Location& location) {
                return mbind(location.city, [](const City& city) {
                    return city.name;
                }).value_or("No city");
            }).value_or("No location");
        }).value_or("No person") << "\n";
    }
}

int main()
{
    {
        using namespace Pointers;

        User user = {
            new Person { "Bob", "Williams", 27,
                new Location {
                    new City { "Canada", "Toronto" },
                    new Address { "The main st.", 1 },
                }
            }, "user@company.com"
        };

        printCity(user);
    }

    {
        using namespace Optionals;

        User user = {
            Person { "Bob", "Williams", 27,
                Location {
                    City { "Canada", "Toronto" },
                    Address { "The main st.", 1 },
                }
            }, "user@company.com"
        };

        printCity(user);
    }

    return 0;
}

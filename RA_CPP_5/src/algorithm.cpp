#include <iostream>
#include <iterator>
#include <sstream>
#include <type_traits>
#include <vector>

namespace TypeTraits
{
    template<typename Callable, typename... Args>
    struct IsPredicate : std::bool_constant<std::is_invocable_r_v<bool, Callable, Args...>> {};
}

namespace algorithm
{
    template<typename InputIterator, typename OutputIterator, typename Predicate, typename Transform>
    OutputIterator transform_if(InputIterator first, InputIterator last, OutputIterator output, Predicate&& pred, Transform&& transform)
    {
        static_assert(TypeTraits::IsPredicate<Predicate, typename std::iterator_traits<InputIterator>::reference>::value);

        static_assert(
            std::is_invocable_r_v<
                typename std::iterator_traits<OutputIterator>::value_type,
                Transform,
                typename std::iterator_traits<InputIterator>::reference
            >
        );

        for (; first != last; ++first) {
            if (pred(*first)) {
                *output++ = transform(*first);
            }
        }

        return output;
    }
}

struct Person
{
    std::string firstName;
    std::string lastName;
    int age;

    Person(std::string_view firstName, std::string_view lastName, int age)
        : firstName(firstName)
        , lastName(lastName)
        , age(age)
    {}
};

int main()
{
    const std::vector<Person> persons = {
        Person("Alice", "Smith", 28),
        Person("Bob", "Johnson", 12),
        Person("Carl", "Austin", 45),
        Person("Harry", "Brooks", 32),
        Person("Jessica", "Turner", 19),
        Person("William", "Gibbs", 33),
        Person("Simon", "Gibbs", 5),
        Person("Oliver", "Philips", 65),
    };

    algorithm::transform_if(persons.begin(), persons.end(), std::ostream_iterator<std::string>(std::cout, "\n"),
        [](const Person& person) -> bool {
            return person.age > 20;
        },
        [](const Person& person) -> std::string {
            std::stringstream sstream;
            sstream << person.firstName << " " << person.lastName << ", " << person.age;

            return sstream.str();
        }
    );

    return 0;
}

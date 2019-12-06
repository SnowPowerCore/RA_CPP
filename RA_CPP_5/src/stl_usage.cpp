#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <string_view>
#include <vector>

struct Person
{
    std::string firstName;
    std::string lastName;
    int age;
};

namespace
{
    std::ostream &operator<<(std::ostream &out, const Person &person)
    {
        return out << person.lastName << " " << person.firstName << " " << person.age << " years old";
    }

    void print(const std::vector<Person> &persons)
    {
        for (const Person &person: persons) {
            std::cout << person << "\n";
        }
    }
}

int main()
{
    std::vector<Person> persons = {
        {"Ivan",     "Ivanov",   23},
        {"Peter",    "Petrov",   23},
        {"Dmitry",   "Dmitriev", 23},
        {"Ivan",     "Dmitriev", 23},
        {"Svetlana", "Ivanova",  33},
        {"Olga",     "Ivanova",  33},
        {"Olga",     "Petrova",  33},
        {"Ivan",     "Ivanov",   47},
    };

    print(persons);

    // NOTE: Сортируем список граждан по фамилии.
    std::sort(persons.begin(), persons.end(), [](const Person &lhs, const Person &rhs) {
        return lhs.lastName < rhs.lastName;
    });

    // NOTE: Тепеь по возрастам фамилии.
    // Обратите внимание, что алгоритм std::stable_sort сохраняет порядок элементов от предыдущей сортировки.
    std::stable_sort(persons.begin(), persons.end(), [](const Person &lhs, const Person &rhs) {
        return lhs.age < rhs.age;
    });

    print(persons);

    std::set<std::string> families;

    // NOTE: Преобразуем вектор граждан во множество фамилий.
    std::transform(persons.cbegin(), persons.cend(),
        std::inserter(families, families.end()), // NOTE: Используем итератор вставки в множество.
        [](const Person &person) {
            return person.lastName;
        }
    );

    // NOTE: Используем алгоритм std::copy и итератор вывода в поток для вывода на экран фамилий граждан.
    std::copy(families.cbegin(), families.cend(), std::ostream_iterator<std::string>(std::cout, "\n"));

    // NOTE: Или мы можем вывести их на экран, использую явную итерацию в цикле for.
    for (auto it = families.cbegin(); it != families.cend(); ++it) {
        std::cout << *it << "\n";
    }

    // NOTE: Или записать то же самое лаконичнее.
    for (std::string_view family : families) {
        std::cout << family << "\n";
    }

    return 0;
}

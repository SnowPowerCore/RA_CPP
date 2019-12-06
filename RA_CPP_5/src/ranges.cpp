#include <iostream>
#include <string>
#include <vector>

#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

namespace TypeTraits
{
    template<typename Callable, typename... Args>
    struct IsPredicate : std::bool_constant<std::is_invocable_r_v<bool, Callable, Args...>> {};
}

namespace algorithm
{
    template<typename InputIterator, typename OutputIterator, typename Predicate, typename Transform>
    OutputIterator transform_if(InputIterator first, InputIterator last, OutputIterator output, Predicate pred, Transform transform)
    {
        for (; first != last; ++first) {
            if (pred(*first)) {
                *output++ = transform(*first);
            }
        }

        return output;
    }

    template<
        typename Containter,
        typename Predicate, REQUIRES(TypeTraits::IsPredicate<Predicate, typename Containter::value_type>::value)
    >
    /**
     * @brief Фильтрует контейнер по указанному условию.
     * @param container исходный контейнер
     * @param predicate предикат условия фильтрации
     * @return новый контейнер, содержащий прошедшие фильтрацию элементы исходного
     */
    Containter filter(const Containter& container, Predicate predicate)
    {
        Containter result;

        for (auto item : container) {
            if (predicate(item)) {
                result.push_back(item);
            }
        }

        return result;
    }

    template<
        typename Containter,
        typename Transform, REQUIRES(std::is_invocable_v<Transform, typename Containter::value_type>),
        typename ResultType = std::vector<std::invoke_result_t<Transform&&, typename Containter::value_type>>
    >
    /**
     * @brief Применяет указанную функцию преобразования к элементам контейнера
     * @param container исходный контейнер
     * @param transform функция преобразования элемена контейнера
     * @return новый контейнер, содержащий преобразованные элементы исходного
     */
    ResultType transform(const Containter& container, Transform&& transform)
    {
        ResultType result;

        for (auto item : container) {
            result.push_back(transform(item));
        }

        return result;
    }
}

/**
 * @struct Number
 * @brief Обёртка над целыми числами для анализа копирований в алгоритмах.
 */
struct Number final
{
    int value;

    Number(int value)
        : value(value)
    {}

    Number(const Number& other)
        : Number(other.value)
    {
        std::cout << "COPY: " << value << "\n";
    }

    operator int() const
    {
        return value;
    }
};

int main()
{
    static const std::vector<Number> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // NOTE: Применяем композицию функций "transform(filter(...))".
    // Обратите внимание на лишнее копирование.
    auto container = algorithm::transform(
        algorithm::filter(numbers, [](int i) { return i % 2 == 0; }),
        [](int i) { return std::to_string(i); }
    );

    for (auto i : container) {
        std::cout << i << " ";
    }

    // NOTE: Применяем заранее заготовленный алгоритм.
    algorithm::transform_if(numbers.begin(), numbers.end(), std::ostream_iterator<std::string>(std::cout, " "),
        [](int i) { return i % 2 == 0; },
        [](int i) { return std::to_string(i); }
    );

    // NOTE: Применяем диапазоны (библиотека range-v3).
    auto range = numbers
        | ranges::views::filter([](int i) { return i % 2 == 0; })
        | ranges::views::transform([](int i) { return std::to_string(i); });

    std::cout << range << "\n";

    for (auto i : range) {
        std::cout << i << " ";
    }

    std::cout << "\n";
}

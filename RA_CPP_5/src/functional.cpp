#include <functional>
#include <iostream>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

using namespace std::placeholders;

namespace
{
    /**
     * @brief Частичное применение функции.
     */
    template<typename Callable, typename... Args>
    constexpr auto parital_apply(Callable&& callable, Args&&... args)
    {
        return [=](auto&&... tail) constexpr {
            return callable(args..., std::forward<decltype(tail)>(tail)...);
        };
    }

    /**
     * @brief Комбинация функций.
     * @note combine(F, f0, ... fn)(x0, ... xn) -> F(f0(x0, ... xn), ... fn(x0, ... xn))
     */
    template<typename F, typename... Fargs>
    constexpr auto combine(F&& f, Fargs&&... fargs)
    {
        return [=](auto&&... args) constexpr {
            return f(fargs(std::forward<decltype(args)>(args)...)...);
        };
    }

    constexpr int f(int a, int b, int c)
    {
        return a + b * c;
    }

    constexpr double add(double a, double b)
    {
        return a + b;
    }

    constexpr double mul(double a, double b)
    {
        return a * b;
    }
}

int main()
{
     auto f1 = parital_apply(f, 1);
     auto f2 = parital_apply(f, 1, 2);
     auto f3 = parital_apply(f, 1, 2, 3);

     std::cout << f(1, 2, 3) << "\n";
     std::cout << f1(2, 3) << "\n";
     std::cout << f2(3) << "\n";
     std::cout << f3() << "\n";

     std::cout << "---" << "\n";

     // NOTE: Записываем сложное выражение функциями высшего порядка.
     // c1(x) -> add(curry(mul, 2)(x), curry(mul, 3)(x))
     //       -> curry(mul, 2)(x) + curry(mul, 3)(x)
     //       -> mul(2, x) + mul(3, x)
     //       -> 2 * x + 3 * x
     auto c1 = combine(add, parital_apply(mul, 2), parital_apply(mul, 3));

     // NOTE: Заменяем наши функции на библиотечные.
     // add -> std::plus
     // mul -> std::multiplies
     // parital_apply -> std::bind
     auto c2 = combine(
         std::plus<>(),
         std::bind(std::multiplies<>(), _1, 2),
         std::bind(std::multiplies<>(), _1, 3)
     );

     std::cout << c1(2) << " " << c2(2) << "\n";
     std::cout << add(parital_apply(mul, 2)(2), parital_apply(mul, 3)(2)) << "\n";

    return 0;
}

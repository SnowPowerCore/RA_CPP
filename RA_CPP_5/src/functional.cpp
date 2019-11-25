#include <functional>
#include <iostream>

using namespace std::placeholders;

namespace
{
    template<typename Callable, typename... Args>
    constexpr auto curry(Callable&& callable, Args&&... args)
    {
        return [=](auto&&... tail) constexpr {
            return callable(args..., std::forward<decltype(tail)>(tail)...);
        };
    }

    template<typename Head, typename... Tail>
    constexpr auto combine(Head&& head, Tail&&... tail)
    {
        return [=](auto&&... args) constexpr {
            return head(tail(std::forward<decltype(args)>(args)...)...);
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
     auto f1 = curry(f, 1);
     auto f2 = curry(f, 1, 2);
     auto f3 = curry(f, 1, 2, 3);

     std::cout << f(1, 2, 3) << "\n";
     std::cout << f1(2, 3) << "\n";
     std::cout << f2(3) << "\n";
     std::cout << f3() << "\n";

     std::cout << "---" << "\n";

     // c1(x) -> add(curry(mul, 2)(x), curry(mul, 3)(x))
     //       -> curry(mul, 2)(x) + curry(mul, 3)(x)
     //       -> mul(2, x) + mul(3, x)
     //       -> 2 * x + 3 * x
     auto c1 = combine(add, curry(mul, 2), curry(mul, 3));

     auto c2 = combine(
         std::plus<>(),
         std::bind(std::multiplies<>(), _1, 2),
         std::bind(std::multiplies<>(), _1, 3)
     );

     std::cout << c1(2) << " " << c2(2) << "\n";
     std::cout << add(curry(mul, 2)(2), curry(mul, 3)(2)) << "\n";

    return 0;
}

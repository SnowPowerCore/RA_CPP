#include <iostream>
#include <string>
#include <thread>

#include "Generator.h"
#include "Task.h"

using namespace std::chrono_literals;

namespace
{
    Task<int> findTheAnswer()
    {
        std::this_thread::sleep_for(5s);
        co_return 42;
    }

    Task<std::string> getTheText()
    {
        std::this_thread::sleep_for(3s);
        co_return "Hello! The answer is ";
    }

    Task<void> run()
    {
        std::cout << "The coroutine is started" << "\n";

        const std::string text = co_await getTheText();
        const int answer = co_await findTheAnswer();

        std::cout << text << answer << "\n";
        std::cout << "The coroutine is finished" << "\n";
    }

    template<typename T>
    Generator<T> range(T first, T last, T step = 1)
    {
        for (T i = first; i < last; i += step) {
            co_yield i;
        }
    }
}

int main()
{
    auto task = run();
    task.resume();

    for (int i : range(10, 26, 3)) {
        std::cout << i << "\n";
    }

    return 0;
}

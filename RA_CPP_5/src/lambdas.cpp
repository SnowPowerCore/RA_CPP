#include <algorithm>
#include <functional>
#include <iostream>
#include <string_view>
#include <vector>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

namespace
{
    void function()
    {
        std::cout << "function() is called" << "\n";
    }

    // NOTE: Функциональный объект. Ведёт себя и как объект и как функция.
    struct FunctionObject
    {
        std::vector<std::string> &log; // NOTE: Состояние объекта (обычное поле класса).

        explicit FunctionObject(std::vector<std::string> &log)
            : log(log)
        {}

        // NOTE: Оператор вызова (можно использовать как функцию).
        [[maybe_unused]] void operator()() const
        {
            log.emplace_back("FunctionObject is called");
        }
    };
}

struct Connection
{
    // NOTE: Инкапсуляция всего того, что ведёт себя как функция заданной сигнатуры, в один тип.
    using Handler = std::function<void()>;

    std::vector<Handler> handlers;
    std::vector<std::string> log;

    // NOTE: Проверяем на наличие оператора вызова и добавляем обработчик события "Connected".
    template<typename Callable, REQUIRES(std::is_invocable_v<Callable>)>
    Connection &onConnected(Callable&& callable)
    {
        handlers.push_back(std::forward<Callable>(callable));
        return *this;
    }

    void connect() const
    {
        std::cout << "Connected" << "\n";

        // NOTE: Событие "Connected" наступило, вызываем обработчики.
        for (const Handler &handler : handlers) {
            handler();
        }
    }

    void printLog()
    {
        std::cout << "\n" << "Log:" << "\n";

        for (std::string_view line : log) {
            std::cout << line << "\n";
        }
    }
};

int main()
{
    {
        Connection connection;

        connection.onConnected(function);
        connection.onConnected(FunctionObject(connection.log));

        connection.onConnected([&log = connection.log]() {
            log.emplace_back("lambda is called");
        });

        connection.connect();
        connection.printLog();
    }

    {
        Connection connection;

        connection.onConnected([]() {
            std::cout << "do something" << "\n";
        }).onConnected([]() {
            std::cout << "do another thing" << "\n";
        }).onConnected([]() {
            std::cout << "do third one" << "\n";
        }).onConnected([]() {
            std::cout << "do something else" << "\n";
        }).connect();
    }

    return 0;
}

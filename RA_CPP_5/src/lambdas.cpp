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

    struct FunctionObject
    {
        std::vector<std::string> &log;

        explicit FunctionObject(std::vector<std::string> &log)
            : log(log)
        {}

        void operator()() const
        {
            log.emplace_back("FunctionObject is called");
        }
    };
}

struct Connection
{
    using Handler = std::function<void()>;

    std::vector<Handler> handlers;
    std::vector<std::string> log;

    template<typename Callable, REQUIRES(std::is_invocable_v<Callable>)>
    Connection &onConnected(Callable&& callable)
    {
        handlers.push_back(std::forward<Callable>(callable));
        return *this;
    }

    void connect() const
    {
        std::cout << "Connected" << "\n";

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

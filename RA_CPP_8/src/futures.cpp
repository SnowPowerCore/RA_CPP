#include <chrono>
#include <iostream>
#include <thread>

#include <boost/lexical_cast.hpp>
#include <boost/thread/future.hpp>

using namespace std::chrono_literals;

// NOTE: Демонстрация возможностей boost::future при работе вводом/выводом.

struct Nothing
{
    constexpr void operator()() const noexcept {}
};

/**
 * @class Writer
 * @brief Эмулятор долгой записи строк в поток вывода.
 */
struct Writer
{
    std::ostream& stream;

    explicit Writer(std::ostream& stream)
        : stream(stream)
    {}

    /**
     * @brief Записывает строку в поток (синхронно и долго).
     */
    void writeLine(std::string_view line)
    {
        std::this_thread::sleep_for(1s);
        stream << line << std::endl;
    }

    /**
     * @brief Асинхронно записывает строку в поток.
     */
    template<typename Callback = Nothing>
    void writeLineAsync(std::string_view line, Callback&& callback = Callback())
    {
        std::thread([line, callback, this] () {
            writeLine(line);
            callback();
        }).detach();
    }
};

namespace
{
    // NOTE: Синхронная последовательная запись.
    // Надёжно, но долго. Вынуждены простаивать из-за ввода/вывода.
    void synchronous()
    {
        Writer writer(std::cout);
        writer.writeLine("line_1");
        writer.writeLine("line_2");
        writer.writeLine("line_3");
        writer.writeLine("line_4");
        writer.writeLine("line_5");

        std::cout << "Other task" << "\n";
    }

    // NOTE: Асинхронная запись.
    // Ненадёжно. Данные пишутся в произвольном порядке.
    void asynchronous()
    {
        Writer writer(std::cout);
        writer.writeLineAsync("line_1");
        writer.writeLineAsync("line_2");
        writer.writeLineAsync("line_3");
        writer.writeLineAsync("line_4");
        writer.writeLineAsync("line_5");

        std::cout << "Other task" << "\n";
        std::this_thread::sleep_for(10s);
    }

    // NOTE: Асинхронная запись с callback-ами.
    // Надёжно и быстро, но не ясно когда запись завершится.
    void callbacks()
    {
        Writer writer(std::cout);

        writer.writeLineAsync("line_1", [&writer] {
            writer.writeLineAsync("line_2", [&writer] {
                writer.writeLineAsync("line_3", [&writer] {
                    writer.writeLineAsync("line_4", [&writer] {
                        writer.writeLineAsync("line_5");
                    });
                });
            });
        });

        std::cout << "Other task" << "\n";
        std::this_thread::sleep_for(10s);
    }

    // NOTE: boost::future. Применяем метод "then()" для связывания цепочки callback-ов из предыдущего примера.
    // Надёжно и быстро, завершения по выходу из метода "get()". Можно навешать любые преобразования.
    void futures()
    {
        Writer writer(std::cout);

        auto future = boost::async([&writer] { writer.writeLine("line_1"); })
            .then([&writer](auto) { writer.writeLine("line_2"); })
            .then([&writer](auto) { writer.writeLine("line_3"); })
            .then([&writer](auto) { writer.writeLine("line_4"); })
            .then([&writer](auto) { writer.writeLine("line_5"); })
            .then([](auto) { return 42; })
            .then([](auto future) { return (future.get() < 30) ? 0 : 100; })
            .then([](auto future) { return boost::lexical_cast<std::string>(future.get()); })
            .then([](auto future) {
                std::string line = future.get();
                std::reverse(line.begin(), line.end());

                return line;
            });

        std::cout << "Other task" << "\n";
        std::cout << future.get() << "\n";
    }
}

int main()
{
    synchronous();
    asynchronous();
    callbacks();
    futures();

    return 0;
}

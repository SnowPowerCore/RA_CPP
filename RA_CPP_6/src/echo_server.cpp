#include <exception>
#include <filesystem>
#include <iostream>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fs = std::filesystem;

using namespace boost::asio;

namespace
{
    template<typename OStream, typename IStream, typename Transform>
    void transform(OStream&& out, IStream&& in, Transform&& transform = Transform())
    {
        for (std::string line; std::getline(in, line);) {
            out << transform(line) << "\n";
        }
    }
}

/**
 * @struct StopServerExeption
 * @brief Пользовательское исключение для остановки сервера.
 */
struct StopServerExeption final : std::exception {};

class Server final
{
public:
    Server() = default;

    /**
     * @brief Выполняет настройку сервера по указанному файлу конфигурации.
     */
    void configure(const fs::path& filename)
    {
        using namespace boost::property_tree;

        // NOTE: Парсим JSON-файл настроек с помощью библиотеки boost.property_tree.
        ptree tree;
        read_json(filename.string(), tree);

        endpoint_ = ip::tcp::endpoint(
            ip::address::from_string(tree.get<std::string>("network.host")),
            tree.get<uint16_t>("network.port")
        );
    }

    /**
     * @brief Запускает сервер.
     * @warning Корректный выход из функции возможем лишь при выбросе исключения.
     */
    [[noreturn]] void start()
    {
        std::cout << "Service started" << std::endl;

        ip::tcp::iostream stream;
        ip::tcp::acceptor acceptor(service_, endpoint_);

        // NOTE: Принимаем входящее соедение и свзываем его с потоком.
        acceptor.accept(*stream.rdbuf());

        while (true) {
            transform(stream, stream, [](std::string_view line) {
                if (line == "SERVER_STOP") {
                    throw StopServerExeption(); // NOTE: Выходим из функции, выбросив исключение остановки сервера.
                }

                return line;
            });
        }
    }

    void stop()
    {
        service_.stop();
        std::cout << "Service stopped" << std::endl;
    }

private:
    ip::tcp::endpoint endpoint_;
    io_service service_;
};

int main()
{
    Server server;

    try {
        server.configure("EchoServer.conf");
        server.start();
    } catch (const StopServerExeption&) {
        // NOTE: Корректно останавливаем сервер и завершаем программу.
        server.stop();
    } catch (const std::exception& exception) {
        // NOTE: Все остальные исключения считаем ошибками. Выводим информацию о них и корректно завершаем программу.
        std::cerr << "Error occurred: " << exception.what() << "\n";
    }

    return 0;
}

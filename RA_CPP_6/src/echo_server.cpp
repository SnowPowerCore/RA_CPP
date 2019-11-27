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

struct StopServerExeption final : std::exception {};

class Server final
{
public:
    Server() = default;

    void configure(const fs::path& filename)
    {
        using namespace boost::property_tree;

        ptree tree;
        read_json(filename.string(), tree);

        endpoint_ = ip::tcp::endpoint(
            ip::address::from_string(tree.get<std::string>("network.host")),
            tree.get<uint16_t>("network.port")
        );
    }

    [[noreturn]] void start()
    {
        std::cout << "Service started" << std::endl;

        ip::tcp::iostream stream;
        ip::tcp::acceptor acceptor(service_, endpoint_);
        acceptor.accept(*stream.rdbuf());

        while (true) {
            transform(stream, stream, [](std::string_view line) {
                if (line == "SERVER_STOP") {
                    throw StopServerExeption();
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
        server.stop();
    } catch (const std::exception& exception) {
        std::cerr << "Error occurred: " << exception.what() << "\n";
    }

    return 0;
}

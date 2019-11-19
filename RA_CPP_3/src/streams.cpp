#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <boost/asio/ip/tcp.hpp>

namespace
{
    std::vector<std::string> split(const std::string& string, char delimiter)
    {
        std::vector<std::string> result;

        std::stringstream sstream(string);

        for (std::string line; std::getline(sstream, line, delimiter);) {
            result.push_back(std::move(line));
        }

        return result;
    }
}

struct Nothing {
    constexpr std::string_view operator()(std::string_view line) const noexcept
    {
        return line;
    }
};

struct CsvReader {
    char delimiter;
    mutable std::vector<std::vector<std::string>> data;

    explicit CsvReader(char delimiter = ',')
        : delimiter(delimiter)
    {}

    std::string_view operator()(const std::string& line) const
    {
        data.push_back(split(line, delimiter));
        return line;
    }
};

namespace
{
    template<typename OStream, typename IStream, typename Transform = Nothing>
    void transform(OStream&& out, IStream&& in, Transform&& transform = Transform())
    {
        for (std::string line; std::getline(in, line);) {
            out << transform(line) << "\n";
        }
    }
}

int main()
{
    std::stringstream sstream;
    sstream << "line_1" << "\n";
    sstream << "line_2" << "\n";
    sstream << "line_3" << "\n";

    // запись текстового файла
    transform(std::ofstream("output.txt"), sstream);

    CsvReader reader;

    // парсинг CSV-файла
    transform(std::ofstream("/dev/null"), std::ifstream("../src/input.csv"), reader);

    for (const std::vector<std::string>& row : reader.data) {
        for (const std::string& string : row) {
            std::cout << string << "|-|";
        }

        std::cout << "\n";
    }

    boost::asio::ip::tcp::iostream socket_stream;
    socket_stream.connect("127.0.0.1", "3333");

    // сетевой обмен с преобразованием в UPPER_CASE
    transform(socket_stream, socket_stream, [](std::string line) {
        std::transform(line.begin(), line.end(), line.begin(), ::toupper);
        return line;
    });
}
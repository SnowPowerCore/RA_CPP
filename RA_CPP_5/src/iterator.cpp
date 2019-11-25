#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

struct ReadLineIterator
{
    std::istream* stream;
    std::string line;

    explicit ReadLineIterator(std::istream* stream = nullptr)
        : stream(stream)
    {}

    ReadLineIterator& operator++()
    {
        if (stream && !std::getline(*stream, line)) {
            stream = nullptr;
        }

        return *this;
    }

    std::string operator*() const
    {
        return line;
    }

    bool operator!=(const ReadLineIterator& other) const
    {
        return (stream != other.stream) || (line != other.line);
    }
};

namespace std
{
    template<>
    struct iterator_traits<ReadLineIterator>
    {
        using iterator_category = std::input_iterator_tag;
        using value_type = std::string;
    };
}

class ReadLines
{
public:
    explicit ReadLines(std::istream& stream)
        : stream_(stream)
    {}

    ReadLineIterator begin()
    {
        return ReadLineIterator(&stream_);
    }

    ReadLineIterator end()
    {
        return ReadLineIterator();
    }

private:
    std::istream& stream_;
};

int main()
{
    std::ifstream file("../../src/CMakeLists.txt");

    for (const std::string& line : ReadLines(file)) {
        std::cout << line << "\n";
    }

    return 0;
}

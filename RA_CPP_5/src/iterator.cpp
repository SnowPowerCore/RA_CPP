#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

/**
 * @struct ReadLineIterator
 * @brief Итератор чтения строк из потока ввода.
 */
struct ReadLineIterator
{
    std::istream* stream;
    std::string line;

    explicit ReadLineIterator(std::istream* stream = nullptr)
        : stream(stream)
    {}

    ReadLineIterator& operator++()
    {
        // NOTE: Каждый итерация - чтение следующей строки.
        if (stream && !std::getline(*stream, line)) {
            stream = nullptr;
        }

        // NOTE: Обратите внимание! Возвращаем тот же объект.
        return *this;
    }

    std::string operator*() const
    {
        return line;
    }

    // WARNING: Считаем, для наглядности, что все строки в потоке уникальны. Не используйте итератор в реально коде!
    bool operator!=(const ReadLineIterator& other) const
    {
        return (stream != other.stream) || (line != other.line);
    }
};

// NOTE: Определяем свойства итератора.
// Нам важны лишь категория и тип элементов последовательности.
namespace std
{
    template<>
    struct iterator_traits<ReadLineIterator>
    {
        using iterator_category = std::input_iterator_tag;
        using value_type = std::string;
    };
}

/**
 * @class ReadLines
 * @brief Последовательность строк в потоке вывода.
 * @note Класс нужен лишь для лаконичной записи цикла for.
 */
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

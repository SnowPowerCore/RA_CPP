#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#define REQUIRES(...) typename = std::enable_if_t<__VA_ARGS__>

using namespace boost::iostreams;

struct Nothing
{
    constexpr std::string_view operator()(std::string_view line) const noexcept
    {
        return line;
    }
};

/**
 * @class RangeStream
 * @brief Stream-подобная обёртка над парой итераторов ввода (последовательностью).
 */
template<typename InputIterator, REQUIRES(std::is_same_v<std::string, typename std::iterator_traits<InputIterator>::value_type>)>
class RangeStream final
{
public:
    explicit RangeStream(InputIterator begin, InputIterator end)
        : begin_(std::move(begin))
        , end_(std::move(end))
    {}

    /**
     * @brief Показывает, достигнут ли конец потока (последовательности).
     */
    bool eof() const
    {
        return (begin_ == end_);
    }

    /**
     * @brief Оператор чтения строк из потока.
     */
    RangeStream& operator>>(std::string& line)
    {
        line = *begin_++;
        return *this;
    }

private:
    InputIterator begin_;
    InputIterator end_;
};

// WARNING: Этот "грязный хак" исключительно для примера. Не повторяйте в домашних условиях!
// std::getline использовать хочется, а наследовать от std::istream - нет.
namespace std
{
    template<typename InputIterator>
    bool getline(RangeStream<InputIterator>& in, std::string& line)
    {
        const bool isEof = in.eof();

        if (!isEof) {
            in >> line;
        }

        return !isEof;
    }
}

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

int main(int argc, char** argv)
{
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " -i <input_file> -o <output_file>" << "\n";
        return 1;
    }

    const std::regex re(R"(https?:\/\/?([\w\.-]+)(\/[\w\.\,\/\+]*)*\/?)");

    std::set<std::string> domains;

    std::ifstream file(argv[2], std::ios_base::in | std::ios_base::binary);

    filtering_streambuf<input> streambuf;
    streambuf.push(gzip_decompressor());
    streambuf.push(file);

    std::istream in(&streambuf);

    // NOTE: Применяем функцию преобразования текстовых потоков ("transform()") для парсинга потока.
    transform(std::ofstream("/dev/null"), in, [&re, &domains](const std::string& line) {
        for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator(); ++it) {
            const std::smatch match = *it;
            domains.insert(match[0]);
        }

        return "";
    });

    file.close();

    // NOTE: И снова применяем функцию "transform()" вывода результатов из коллекции в файл.
    transform(std::ofstream(argv[4]), RangeStream(domains.cbegin(), domains.cend()));

    return 0;
}

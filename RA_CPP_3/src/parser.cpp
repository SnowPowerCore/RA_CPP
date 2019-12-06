#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace boost::iostreams;

int main(int argc, char** argv)
{
    // NOTE: Обрабатываем аргументы командной строки.
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " -i <input_file> -o <output_file>" << "\n";
        return 1;
    }

    // NOTE: Регулярное выражения для поиска URL-лов.
    static const std::regex re(R"(https?:\/\/?([\w\.-]+)(\/[\w\.\,\/\+]*)*\/?)");

    // NOTE: Множество используем для удаления дубликатов и сортировки URL-лов по алфавиту.
    std::set<std::string> urls;

    std::string line;

    // NOTE: Открываем архив с логами на чтение.
    std::ifstream file(argv[2], std::ios::in | std::ios::binary);

    // NOTE: Устанавливаем буфер потока с распаковкой архива на лету.
    filtering_streambuf<input> streambuf;
    streambuf.push(gzip_decompressor());
    streambuf.push(file);

    std::istream in(&streambuf);

    while (std::getline(in, line)) {
        // NOTE: Для каждой строки собираем все вхождения URL-лов.
        for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator(); ++it) {
            const std::smatch match = *it;
            urls.insert(match[0]);
        }
    }

    file.close();

    // NOTE: Открываем выходной файл на запись.
    std::ofstream out(argv[4], std::ios::out);

    for (const std::string& domain : urls) {
        out << domain << "\n";
    }

    out.close();

    return 0;
}

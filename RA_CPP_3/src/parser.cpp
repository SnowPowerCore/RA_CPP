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
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " -i <input_file> -o <output_file>" << "\n";
        return 1;
    }

    const std::regex re(R"(https?:\/\/?([\w\.-]+)(\/[\w\.\,\/\+]*)*\/?)");
    std::set<std::string> domains;
    std::string line;

    std::ifstream file(argv[2], std::ios_base::in | std::ios_base::binary);

    filtering_streambuf<input> streambuf;
    streambuf.push(gzip_decompressor());
    streambuf.push(file);

    std::istream in(&streambuf);

    while (std::getline(in, line)) {
        for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator(); ++it) {
            const std::smatch match = *it;
            domains.insert(match[0]);
        }
    }

    file.close();

    std::ofstream out(argv[4]);

    for (const std::string& domain : domains) {
        out << domain << "\n";
    }

    out.close();

    return 0;
}

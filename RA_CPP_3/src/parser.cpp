#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <set>

int main(int argc, char** argv)
{
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " -i <input_file> -o <output_file>" << "\n";
        return 1;
    }

    const std::regex re(R"(https?:\/\/?([\w\.-]+)(\/[\w\.\,\/\+]*)*\/?)");
    std::set<std::string> domains;
    std::string line;
    std::ifstream in(argv[2]);

    while (std::getline(in, line)) {
        for (auto it = std::sregex_iterator(line.begin(), line.end(), re); it != std::sregex_iterator(); ++it) {
            const std::smatch match = *it;
            domains.insert(match[0]);
        }
    }

    in.close();

    std::ofstream out(argv[4]);

    for (const std::string& domain : domains) {
        out << domain << "\n";
    }

    out.close();

    return 0;
}

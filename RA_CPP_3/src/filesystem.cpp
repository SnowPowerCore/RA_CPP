#include <filesystem>
#include <iostream>
#include <sstream>

namespace fs = std::filesystem;

struct FileInfo final {
    bool isDirectory;
    std::string name;
    uint64_t size;
    fs::perms permissions;

    explicit FileInfo(const fs::path& path)
        : isDirectory(fs::is_directory(path))
        , name(path.filename().generic_string())
        , size(isDirectory ? 0 : fs::file_size(path))
        , permissions(fs::status(path).permissions())
    {}
};

namespace
{
    std::ostream& operator<<(std::ostream& out, fs::perms p)
    {
        return out << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
                   << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
                   << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
                   << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
                   << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
                   << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
                   << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
                   << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
                   << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
    }

    [[maybe_unused]] std::ostream& operator<<(std::ostream& out, const FileInfo& fileInfo)
    {
        return out << (fileInfo.isDirectory ? 'd' : '-')
                   << fileInfo.permissions << "\t"
                   << fileInfo.size << "\t"
                   << fileInfo.name << "\n";
    }

    std::string displayPath(uint8_t depth, const fs::path& path)
    {
        const std::string name = path.filename().generic_string();

        if (depth == 0) {
            return name;
        }

        const char ch = fs::is_directory(path) ? '-' : ' ';

        std::stringstream stream;
        stream << '|' << std::string(depth, ch) << std::string(depth, ch) << " " << name;

        return stream.str();
    }
}

struct TreePrinter {
    uint8_t depth = 0;

    void print(const fs::path& path)
    {
        std::cout << displayPath(depth, path) << "\n";

        if (fs::is_directory(path)) {
            ++depth;

            for (const fs::path& path : fs::directory_iterator(path)) {
                print(path);
            }

            --depth;
        }
    }
};

int main()
{
     for (const fs::path& path : fs::directory_iterator(fs::current_path())) {
         std::cout << FileInfo(path);
     }

     TreePrinter().print(fs::current_path());

    return 0;
}

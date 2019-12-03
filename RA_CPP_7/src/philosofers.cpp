#include <algorithm>
#include <iostream>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

class Philosofer final
{
public:
    Philosofer(std::string_view name, uint8_t left, uint8_t right)
        : name_(name)
        , forks_(std::make_pair(left, right))
    {}

    void eat(std::vector<std::mutex>& forks) const
    {
        const std::scoped_lock lock(forks[forks_.first], forks[forks_.second]);

        printLine(name_ + " has started eating. ");
        std::this_thread::sleep_for(5s);
        printLine(name_ + " has finished eating.");
    }

private:
    static void printLine(std::string_view line)
    {
        const std::lock_guard lock(mutex_);
        std::cout << line << std::endl;
    }

private:
    std::string name_;
    std::pair<uint8_t, uint8_t> forks_;

    static inline std::mutex mutex_;
};

namespace
{
    std::vector<Philosofer> philosofers(size_t count)
    {
        std::vector<Philosofer> philosofers;

        std::generate_n(std::back_inserter(philosofers), count, [count, &philosofers]() {
            const auto index = static_cast<uint8_t>(philosofers.size());
            return Philosofer("P" + std::to_string(index), index, ((index + 1) % count));
        });

        return philosofers;
    }
}

int main()
{
    const std::vector philosofers = ::philosofers(5);
    std::vector<std::mutex> forks(philosofers.size());
    std::vector<std::thread> threads;

    std::transform(philosofers.cbegin(), philosofers.cend(), std::back_inserter(threads),
        [&forks](const Philosofer& philosofer) {
            return std::thread([&philosofer, &forks]{
                philosofer.eat(forks);
            });
        }
    );

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return 0;
}

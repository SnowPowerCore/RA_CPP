#include <chrono>
#include <iostream>

using namespace std::chrono;

class TimeTracker final
{
public:
    explicit TimeTracker(std::string_view label)
        : label_(label)
          , begin_(steady_clock::now())
    {}

    ~TimeTracker()
    {
        std::cout << label_
                  << ": "
                  << duration_cast<milliseconds>(steady_clock::now() - begin_).count()
                  << " ms. elapsed"
                  << "\n";
    }

private:
    std::string label_;
    std::chrono::steady_clock::time_point begin_;
};

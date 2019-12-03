#include <algorithm>
#include <execution>
#include <iostream>
#include <random>
#include <vector>

#include "TimeTracker.h"

int main()
{
    std::vector<long double> vector(100'000'000);

    std::random_device device;
    std::mt19937 generator(device());

    {
        std::generate(vector.begin(), vector.end(), generator);

        TimeTracker tt("seq");
        std::sort(std::execution::seq, vector.begin(), vector.end());
    }

    {
        std::generate(vector.begin(), vector.end(), generator);

        TimeTracker tt("par");
        std::sort(std::execution::par, vector.begin(), vector.end());
    }

// NOTE: This policy is from C++20, but GCC already support it.
#ifdef __GNUC__
    {
        std::generate(vector.begin(), vector.end(), generator);

        TimeTracker tt("unseq");
        std::sort(std::execution::unseq, vector.begin(), vector.end());
    }
#endif

    {
        std::generate(vector.begin(), vector.end(), generator);

        TimeTracker tt("par_unseq");
        std::sort(std::execution::par_unseq, vector.begin(), vector.end());
    }

    return 0;
}

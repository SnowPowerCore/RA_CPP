#include <algorithm>
#include <execution>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "TimeTracker.h"

int main()
{
    std::vector<std::string> log;

    std::ifstream file("access.log");

    for (std::string line; std::getline(file, line);) {
        log.push_back(std::move(line));
    }

    TimeTracker tt("transform_reduce");

    // NOTE: Используем алгоритм "transorm_reduce()" с распараллеливанием для организации вычислений по модели "MapReduce".
    std::cout << std::transform_reduce(std::execution::par_unseq, log.cbegin(), log.cend(), 0,
         // Reduce
        [](int lhs, int rhs) {
            return std::max(lhs, rhs);
        },
        // Map
        [](std::string_view line) {
            for (volatile int i = 0; i < 10'000; ++i) {}
            return std::hash<std::string_view>()(line);
        }
    ) << "\n";

    return 0;
}

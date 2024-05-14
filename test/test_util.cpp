#include "test_util.h"

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

std::vector<std::int32_t> generateRandomInts() {
    constexpr static int DATA_SIZE = 100000;

    std::vector<std::int32_t> randomData(DATA_SIZE);
    for (auto &item : randomData) {
        item = rnd();
    }

    return randomData;
}

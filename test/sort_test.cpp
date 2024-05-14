#include "doctest/doctest.h"
#include "sorter.h"
#include "test_util.h"
#include "tmp_file.h"

#include <algorithm>
#include <fstream>
#include <sstream>

void checkSort(std::vector<std::int32_t> data) {
    TmpFile input;
    TmpFile output;

    std::ofstream inputStream(input.get_name());
    for (const auto &item : data) {
        inputStream << item << ' ';
    }
    inputStream << '\n';
    inputStream.flush();
    inputStream.close();

    sort(data.begin(), data.end());

    sort(input.get_name(), output.get_name());
    std::ifstream outputStream(output.get_name());

    std::int32_t value;
    for (const auto &item : data) {
        if (!(outputStream >> value)) {
            FAIL("not enogh elements after sort");
        }

        if (item != value) {
            std::stringstream ss;
            ss << item << " not equal " << value;
            FAIL(ss.str());
        }
    }

    if (outputStream >> value) {
        FAIL("find new elements after sort");
    }
}

TEST_CASE("Small sort test") {
    std::vector<std::int32_t> data = {1, 10, 2, 5, 9};
    checkSort(data);
}

TEST_CASE("Large sort test") {
    constexpr static int TEST_COUNT = 3;

    for (int test = 0; test < TEST_COUNT; test++) {
        auto data = generateRandomInts();
        checkSort(data);
    }
}

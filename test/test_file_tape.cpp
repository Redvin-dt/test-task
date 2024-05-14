#include "doctest/doctest.h"
#include "file_input_tape.h"
#include "file_output_tape.h"
#include "test_util.h"
#include "tmp_file.h"

#include <cstdint>
#include <vector>

TEST_CASE("Tape simple test") {

    auto data = generateRandomInts();

    TmpFile testFile;
    auto outputTape = tape::FileOutputTape::create(testFile.get_name());
    for (const auto &item : data) {
        outputTape->write(item);
        CHECK(outputTape->moveForward());
    }

    auto inputTape = tape::FileInputTape::create(testFile.get_name());
    int elementNum = 0;
    while (inputTape->hasNext()) {
        std::int32_t value = inputTape->read();
        CHECK(elementNum < data.size());
        CHECK(value == data[elementNum]);
        CHECK(inputTape->moveForward());
        elementNum++;
    }

    CHECK(elementNum == data.size());
}

TEST_CASE("Tape move test") {

    auto data = generateRandomInts();

    TmpFile testFile;
    auto outputTape = tape::FileOutputTape::create(testFile.get_name());
    for (const auto &item : data) {
        outputTape->write(item);
        CHECK(outputTape->moveForward());
    }

    auto inputTape = tape::FileInputTape::create(testFile.get_name());
    CHECK(inputTape->hasNext());
    CHECK(!inputTape->moveBackward());

    // check read dont move head of tape
    CHECK(inputTape->read() == data[0]);
    CHECK(inputTape->read() == data[0]);

    for (int i = 0; i < data.size(); i++) {
        CHECK(inputTape->moveForward());
    }

    CHECK(!inputTape->hasNext());
    CHECK(!inputTape->moveForward());
}

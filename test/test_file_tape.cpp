#include "doctest/doctest.h"
#include "file_tape.h"
#include "test_util.h"
#include "tmp_file.h"

#include <cstdint>
#include <vector>

TEST_CASE("Tape simple test") {
    auto data = generateRandomInts();

    TmpFile testFile;
    auto tape = tape::FileTape::create(testFile.get_name());
    for (const auto &item : data) {
        tape->write(item);
        tape->moveForward();
    }

    tape->resetTape();
    int elementNum = 0;
    while (tape->hasNext()) {
        std::int32_t value = tape->read();
        CHECK(elementNum < data.size());
        CHECK(value == data[elementNum]);
        tape->moveForward();
        elementNum++;
    }

    CHECK(elementNum == data.size());
}

TEST_CASE("Tape move test") {
    auto data = generateRandomInts();

    TmpFile testFile;
    auto tape = tape::FileTape::create(testFile.get_name());
    for (const auto &item : data) {
        tape->write(item);
        tape->moveForward();
    }

    tape->resetTape();
    CHECK(tape->hasNext());

    tape->moveForward();
    tape->moveBackward();
    CHECK(tape->read() == data[0]);

    // check read dont move head of tape
    CHECK(tape->read() == data[0]);
    CHECK(tape->read() == data[0]);

    for (int i = 0; i < data.size(); i++) {
        tape->moveForward();
    }

    CHECK(!tape->hasNext());

    tape->resetTape();
    CHECK(tape->hasNext());
    CHECK(tape->read() == data[0]);
}

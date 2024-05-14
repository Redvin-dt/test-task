#include "sorter.h"

#include "file_input_tape.h"
#include "file_output_tape.h"
#include "i_input_tape.h"
#include "i_output_tape.h"
#include "tmp_file.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <unistd.h>
#include <vector>

namespace {

TmpFile merge(TmpFile first, TmpFile second) {
    TmpFile result;
    auto firstTape = tape::FileInputTape::create(first.get_name());
    auto secondTape = tape::FileInputTape::create(second.get_name());
    auto resultTape = tape::FileOutputTape::create(result.get_name());

    while (firstTape->hasNext() || secondTape->hasNext()) {
        if (!secondTape->hasNext()) {
            resultTape->write(firstTape->read());
            resultTape->moveForward();
            firstTape->moveForward();
            continue;
        }

        if (!firstTape->hasNext()) {
            resultTape->write(secondTape->read());
            resultTape->moveForward();
            secondTape->moveForward();
            continue;
        }

        auto firstValue = firstTape->read();
        auto secondValue = secondTape->read();

        if (firstValue < secondValue) {
            resultTape->write(firstValue);
            resultTape->moveForward();
            firstTape->moveForward();
        } else {
            resultTape->write(secondValue);
            resultTape->moveForward();
            secondTape->moveForward();
        }
    }

    return result;
}

TmpFile mergeSort(std::vector<TmpFile> &&files) { // TODO: mb add more func
    if (files.size() == 1) {
        return std::move(files.back());
    }

    std::vector<TmpFile> firstPart;
    std::vector<TmpFile> secondPart;

    for (int i = 0; i < files.size(); i++) {
        if (i < files.size() / 2) {
            firstPart.emplace_back(std::move(files[i]));
        } else {
            secondPart.emplace_back(std::move(files[i]));
        }
    }

    auto threadFunc = [&]() {
        TmpFile first =
            mergeSort(std::move(firstPart)); // TODO: rename and mb parralel
        return first;
    };

    auto future = std::async(threadFunc);
    TmpFile second = mergeSort(std::move(secondPart));
    TmpFile first = future.get();

    return merge(std::move(first), std::move(second));
}

TmpFile dumpBuffer(std::vector<std::int32_t> &buffer) { // TODO: mb rename
    TmpFile result;
    std::sort(buffer.begin(), buffer.end());

    auto output_tape = tape::FileOutputTape::create(result.get_name());

    for (const auto &item : buffer) {
        output_tape->write(item);
        output_tape->moveForward();
    }

    buffer.clear();

    return result;
}

} // namespace

void sort(const std::string &inputFilename, const std::string &outputFilename) {

    constexpr static int BUFFER_SIZE = 8192 / sizeof(std::int32_t);

    std::ifstream inputStream(inputFilename);
    std::ofstream outputStream(outputFilename);

    std::vector<std::int32_t> buffer;
    buffer.reserve(BUFFER_SIZE);

    std::vector<TmpFile> tmpFiles;
    std::int32_t value;
    while (inputStream >> value) {
        buffer.emplace_back(value);

        if (buffer.size() == BUFFER_SIZE) {
            tmpFiles.emplace_back(dumpBuffer(buffer));
        }
    }

    if (!buffer.empty()) {
        tmpFiles.emplace_back(dumpBuffer(buffer));
    }

    auto result = mergeSort(std::move(tmpFiles));
    auto resultTape = tape::FileInputTape::create(result.get_name());

    while (resultTape->hasNext()) {
        outputStream << resultTape->read() << ' ';
        resultTape->moveForward();
    }
    outputStream << '\n';
}

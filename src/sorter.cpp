#include "sorter.h"

#include "file_input_tape.h"
#include "file_output_tape.h"
#include "i_input_tape.h"
#include "i_output_tape.h"
#include "tmp_file.h"

#include <algorithm>
#include <cstdlib>
#include <future>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>

namespace {

constexpr char FILENAME_TEMPLATE[25] =
    "/tmp/tape_storage.XXXXXX"; // TODO: mb remove

TmpFile merge_sort(std::vector<TmpFile> &&files) { // TODO: mb add more func
    if (files.size() == 1) {
        return std::move(files.back());
    }

    std::vector<TmpFile> first_part;
    std::vector<TmpFile> second_part;

    for (int i = 0; i < files.size(); i++) {
        if (i < files.size() / 2) {
            first_part.emplace_back(std::move(files[i]));
        } else {
            second_part.emplace_back(std::move(files[i]));
        }
    }

    TmpFile first =
        merge_sort(std::move(first_part)); // TODO: rename and mb parralel
    TmpFile second = merge_sort(std::move(second_part));

    TmpFile result;
    auto firstTape = tape::FileInputTape::create(first.get_name());
    auto secondTape = tape::FileInputTape::create(second.get_name());
    auto resultTape = tape::FileOutputTape::create(result.get_name());

    while (firstTape->has_value() || secondTape->has_value()) {
        if (!secondTape->has_value()) {
            resultTape->write(firstTape->read());
            resultTape->go_next();
            firstTape->go_next();
            continue;
        }

        if (!firstTape->has_value()) {
            resultTape->write(secondTape->read());
            resultTape->go_next();
            secondTape->go_next();
            continue;
        }

        auto firstValue = firstTape->read();
        auto secondValue = secondTape->read();

        if (firstValue < secondValue) {
            resultTape->write(firstValue);
            resultTape->go_next();
            firstTape->go_next();
        } else {
            resultTape->write(secondValue);
            resultTape->go_next();
            secondTape->go_next();
        }
    }

    return result;
}

TmpFile dump_buffer(std::vector<std::int32_t> &buffer) { // TODO: mb rename
    TmpFile result;
    std::sort(buffer.begin(), buffer.end());

    auto output_tape = tape::FileOutputTape::create(result.get_name());

    for (const auto &item : buffer) {
        output_tape->write(item);
        output_tape->go_next();
    }

    buffer.clear();

    return result;
}

} // namespace

void sort(const std::string &input_filename,
          const std::string &output_filename) {

    constexpr static int BUFFER_SIZE = 8192 / sizeof(std::int32_t);

    auto input_tape = tape::FileInputTape::create(input_filename);
    auto output_tape = tape::FileOutputTape::create(output_filename);

    std::vector<std::int32_t> buffer;
    buffer.reserve(BUFFER_SIZE);

    std::vector<TmpFile> tmp_files;
    while (input_tape->has_value()) {
        buffer.emplace_back(input_tape->read());

        if (buffer.size() == BUFFER_SIZE) {
            tmp_files.emplace_back(dump_buffer(buffer));
        }

        input_tape->go_next();
    }

    if (!buffer.empty()) {
        tmp_files.emplace_back(dump_buffer(buffer));
    }

    auto result = merge_sort(std::move(tmp_files));
    auto resultTape = tape::FileInputTape::create(result.get_name());

    while (resultTape->has_value()) {
        output_tape->write(resultTape->read());
        output_tape->go_next();
        resultTape->go_next();
    }
}

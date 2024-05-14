#include "sorter.h"

#include "file_tape.h"
#include "i_tape.h"
#include "tmp_file.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <vector>

namespace {
void dumpBuffer(std::vector<std::int32_t> &buffer,
                const std::shared_ptr<tape::ITape> &tape) {
    sort(buffer.begin(), buffer.end());

    for (const auto &item : buffer) {
        tape->write(item);
        tape->moveForward();
    }

    buffer.clear();
}

constexpr int BUFFER_SIZE = 8192 / sizeof(std::int32_t);

void copyTape(std::size_t n, const std::shared_ptr<tape::ITape> &src,
              const std::shared_ptr<tape::ITape> &dst) {
    for (std::size_t i = 0; i < n; ++i) {
        dst->write(src->read());
        src->moveForward();
        dst->moveForward();
    }
}

void merge(std::size_t leftSize, std::size_t rightSize,
           const std::shared_ptr<tape::ITape> &leftTape,
           const std::shared_ptr<tape::ITape> &rightTape,
           const std::shared_ptr<tape::ITape> &outputTape) {

    std::size_t l = 0;
    std::size_t r = 0;

    while (l < leftSize || r < rightSize) {
        if (l == leftSize) {
            copyTape(1, rightTape, outputTape);
            r++;
        } else if (r == rightSize) {
            copyTape(1, leftTape, outputTape);
            l++;
        } else {
            std::int32_t firstVal = leftTape->read();
            std::int32_t secondVal = rightTape->read();

            if (firstVal < secondVal) {
                copyTape(1, leftTape, outputTape);
                l++;
            } else {
                copyTape(1, rightTape, outputTape);
                r++;
            }
        }
    }
}

void mergeSort(std::size_t n, std::shared_ptr<tape::ITape> inputData,
               std::shared_ptr<tape::ITape> outputData) {

    TmpFile firstTmpBuf;
    TmpFile secondTmpBuf;

    auto leftTape = tape::FileTape::create(firstTmpBuf.get_name());
    auto rightTape = tape::FileTape::create(secondTmpBuf.get_name());

    for (std::size_t width = BUFFER_SIZE; width < n; width *= 2) {
        inputData->resetTape();
        outputData->resetTape();

        for (std::size_t i = 0; i < n; i += 2 * width) {
            leftTape->resetTape();
            rightTape->resetTape();

            std::size_t leftSize = std::min(n, i + width) - i;
            std::size_t rightSize = 0;

            if (std::min(n, i + 2 * width) >= i + width) {
                rightSize = std::min(n, i + 2 * width) - i - width;
            }

            copyTape(leftSize, inputData, leftTape);
            copyTape(rightSize, inputData, rightTape);

            leftTape->resetTape();
            rightTape->resetTape();

            merge(leftSize, rightSize, leftTape, rightTape, outputData);
        }

        inputData->resetTape();
        outputData->resetTape();
        copyTape(n, outputData, inputData);
    }

    inputData->resetTape();
    outputData->resetTape();
    copyTape(n, inputData, outputData);
}

} // namespace

void sort(const std::string &inputFilename, const std::string &outputFilename) {
    std::ifstream inputStream(inputFilename);
    std::ofstream outputStream(outputFilename);

    std::vector<std::int32_t> buffer;
    buffer.reserve(BUFFER_SIZE);

    TmpFile inputData;
    auto inputTape = tape::FileTape::create(inputData.get_name());
    TmpFile outputData;
    auto outputTape = tape::FileTape::create(outputData.get_name());

    std::int32_t value;
    std::size_t n = 0;
    while (inputStream >> value) {
        buffer.emplace_back(value);

        if (buffer.size() == BUFFER_SIZE) {
            dumpBuffer(buffer, inputTape);
        }
        ++n;
    }

    if (!buffer.empty()) {
        dumpBuffer(buffer, inputTape);
    }

    mergeSort(n, inputTape, outputTape);

    outputTape->resetTape();
    while (outputTape->hasNext()) {
        outputStream << outputTape->read() << ' ';
        outputTape->moveForward();
    }
    outputStream << '\n';
}

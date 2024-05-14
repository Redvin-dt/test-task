#include "file_input_tape.h"
#include "i_input_tape.h"
#include <ios>
#include <memory>

namespace tape {

FileInputTape::FileInputTape(const std::string &filename)
    : fStream_(filename, std::ios_base::binary) {}

std::shared_ptr<IInputTape> FileInputTape::create(const std::string &filename) {
    return std::make_shared<FileInputTape>(filename);
}

FileInputTape::~FileInputTape() { fStream_.close(); }

std::int32_t FileInputTape::read() {
    if (isEof()) {
        return 0;
    }

    std::int32_t result = 0;
    fStream_.read(reinterpret_cast<char *>(&result), sizeof(result));
    moveBackward();
    return result;
}

bool FileInputTape::moveForward() {
    if (isEof()) {
        return false;
    }

    fStream_.seekg(sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

bool FileInputTape::moveBackward() {
    if (fStream_.tellg() == 0) {
        return false;
    }

    fStream_.seekg(-sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

bool FileInputTape::hasNext() { return !isEof(); }

bool FileInputTape::isEof() { return fStream_.peek() == EOF; }

} // namespace tape

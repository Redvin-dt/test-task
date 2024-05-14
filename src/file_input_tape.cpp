#include "file_input_tape.h"
#include "config.h"
#include "i_input_tape.h"
#include <ios>
#include <memory>
#include <thread>

namespace tape {

FileInputTape::FileInputTape(const std::string &filename)
    : fStream_(filename, std::ios_base::binary),
      config_(Config::getInstance()) {}

std::shared_ptr<IInputTape> FileInputTape::create(const std::string &filename) {
    return std::make_shared<FileInputTape>(filename);
}

FileInputTape::~FileInputTape() { fStream_.close(); }

std::int32_t FileInputTape::read() {
    std::this_thread::sleep_for(config_->getReadDelay());
    if (isEof()) {
        return 0;
    }

    std::int32_t result = 0;
    fStream_.read(reinterpret_cast<char *>(&result), sizeof(result));
    moveBackward();
    return result;
}

bool FileInputTape::moveForward() {
    std::this_thread::sleep_for(config_->getMoveDelay());
    if (isEof()) {
        return false;
    }

    fStream_.seekg(sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

bool FileInputTape::moveBackward() {
    std::this_thread::sleep_for(config_->getMoveDelay());
    if (fStream_.tellg() == 0) {
        return false;
    }

    fStream_.clear();
    fStream_.seekg(-sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

void FileInputTape::resetTape() {
    std::this_thread::sleep_for(config_->getResetTapeDelay());
    fStream_.clear();
    fStream_.seekg(0);
}

bool FileInputTape::hasNext() { return !isEof(); }

bool FileInputTape::isEof() { return fStream_.peek() == EOF; }

} // namespace tape

#include "file_output_tape.h"
#include "config.h"
#include <ios>
#include <iostream>
#include <thread>

namespace tape {

FileOutputTape::FileOutputTape(const std::string &filename)
    : fStream_(filename, std::ios::trunc | std::ios::binary),
      config_(Config::getInstance()) {}

FileOutputTape::~FileOutputTape() {
    fStream_.flush();
    fStream_.close();
}

std::shared_ptr<IOutputTape>
FileOutputTape::create(const std::string &filename) {
    return std::make_shared<FileOutputTape>(filename);
}

void FileOutputTape::write(std::int32_t value) {
    std::this_thread::sleep_for(config_->getWriteDelay());

    fStream_.write(reinterpret_cast<char *>(&value), sizeof(value));
    moveBackward();
}

bool FileOutputTape::moveForward() {
    std::this_thread::sleep_for(config_->getMoveDelay());
    if (fStream_.eof()) {
        return false;
    }
    fStream_.seekp(sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

bool FileOutputTape::moveBackward() {
    std::this_thread::sleep_for(config_->getMoveDelay());
    if (fStream_.tellp() == 0) {
        return false;
    }

    fStream_.clear();
    fStream_.seekp(-sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

void FileOutputTape::resetTape() {
    std::this_thread::sleep_for(config_->getResetTapeDelay());
    fStream_.clear();
    fStream_.seekp(0);
}

} // namespace tape

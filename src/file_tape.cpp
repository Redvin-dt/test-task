#include "file_tape.h"
#include "config.h"
#include <fstream>
#include <ios>
#include <memory>
#include <thread>

namespace tape {

FileTape::FileTape(const std::string &filename)
    : fStream_(filename, std::fstream::in | std::fstream::out |
                             std::fstream::binary | std::ios_base::trunc),
      config_(Config::getInstance()), current_position_(std::fstream::beg) {}

std::shared_ptr<ITape> FileTape::create(const std::string &filename) {
    return std::make_shared<FileTape>(filename);
}

FileTape::~FileTape() { fStream_.close(); }

void FileTape::write(std::int32_t value) {
    std::this_thread::sleep_for(config_->getWriteDelay());

    fStream_.write(reinterpret_cast<char *>(&value), sizeof(value));
    setPosition();
}

std::int32_t FileTape::read() {
    std::this_thread::sleep_for(config_->getReadDelay());

    std::int32_t result = 0;
    fStream_.read(reinterpret_cast<char *>(&result), sizeof(result));
    setPosition();
    return result;
}

void FileTape::moveForward() {
    std::this_thread::sleep_for(config_->getMoveDelay());

    current_position_ += std::streamoff(sizeof(VALUE_TYPE));
    setPosition();
}

void FileTape::moveBackward() {
    std::this_thread::sleep_for(config_->getMoveDelay());

    current_position_ -= std::streamoff(sizeof(VALUE_TYPE));
    fStream_.clear();
    setPosition();
}

void FileTape::resetTape() {
    std::this_thread::sleep_for(config_->getResetTapeDelay());
    current_position_ = std::fstream::beg;
    fStream_.clear();
    setPosition();
}

void FileTape::setPosition() {
    fStream_.seekg(current_position_);
    fStream_.seekp(current_position_);
}

bool FileTape::hasNext() { return !isEof(); }

bool FileTape::isEof() { return fStream_.peek() == EOF; }

} // namespace tape

#include "file_output_tape.h"
#include <ios>
#include <iostream>

namespace tape {

FileOutputTape::FileOutputTape(const std::string &filename)
    : fStream_(filename, std::ios::trunc | std::ios::binary) {}

FileOutputTape::~FileOutputTape() {
    fStream_.flush();
    fStream_.close();
}

std::shared_ptr<IOutputTape>
FileOutputTape::create(const std::string &filename) {
    return std::make_shared<FileOutputTape>(filename);
}

void FileOutputTape::write(std::int32_t value) {
    fStream_.write(reinterpret_cast<char *>(&value), sizeof(value));
    moveBackward();
}

bool FileOutputTape::moveForward() {
    if (fStream_.eof()) { // TODO: do smth with eof
        return false;
    }
    fStream_.seekp(sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

bool FileOutputTape::moveBackward() {
    if (fStream_.tellp() == 0) {
        return false;
    }
    fStream_.seekp(-sizeof(VALUE_TYPE), std::ios_base::cur);
    return true;
}

} // namespace tape

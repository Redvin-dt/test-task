#include "file_output_tape.h"

namespace tape {

FileOutputTape::FileOutputTape(const std::string &filename)
    : fStream_(filename, std::ios::trunc), write_buf() {}

FileOutputTape::~FileOutputTape() {
    if (write_buf.has_value()) {
        fStream_ << write_buf.value() << ' ';
    }
    fStream_ << '\n';
}

std::shared_ptr<IOutputTape>
FileOutputTape::create(const std::string &filename) {
    return std::make_shared<FileOutputTape>(filename);
}

bool FileOutputTape::go_next() {
    write_to_file();
    write_buf.reset();
    return true;
}

void FileOutputTape::write(std::int32_t value) { write_buf.emplace(value); }

void FileOutputTape::write_to_file() {
    if (write_buf.has_value()) {
        fStream_ << write_buf.value() << ' ';
    } else {
        fStream_ << 0 << ' ';
    }
}

} // namespace tape

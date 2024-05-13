#include "file_input_tape.h"
#include "i_input_tape.h"
#include <memory>

namespace tape {

FileInputTape::FileInputTape(const std::string &filename)
    : fStream_(filename), last_int() {
    std::int32_t new_value;
    if (fStream_ >> new_value) {
        last_int.emplace(new_value);
    } else {
        last_int.reset(); // TODO: mb union with go_next
    }
}

std::shared_ptr<IInputTape> FileInputTape::create(const std::string &filename) {
    return std::make_shared<FileInputTape>(filename);
}

bool FileInputTape::go_next() {
    std::int32_t new_value;
    if (fStream_ >> new_value) {
        last_int.emplace(new_value);
        return true;
    }
    last_int.reset();

    return false;
}

std::int32_t FileInputTape::read() {
    if (last_int.has_value()) {
        return last_int.value();
    } else {
        return 0;
    }
}

bool FileInputTape::has_value() { return last_int.has_value(); }

} // namespace tape

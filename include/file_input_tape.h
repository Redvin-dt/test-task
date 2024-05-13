#pragma once

#include "i_input_tape.h"

#include <cstddef>
#include <fstream>
#include <memory>
#include <optional>
#include <string>

namespace tape {

class FileInputTape : public IInputTape {
  public:
    FileInputTape(const std::string &filename);

    FileInputTape(const FileInputTape &) = delete;
    FileInputTape(FileInputTape &&) = default;
    FileInputTape &operator=(const FileInputTape &) = delete;
    FileInputTape &operator=(FileInputTape &&) = default;

    static std::shared_ptr<IInputTape> create(const std::string &filename);

    virtual bool go_next() override;
    virtual std::int32_t read() override;
    virtual bool has_value() override;

  private:
    std::optional<std::int32_t> last_int;
    std::ifstream fStream_;
};

} // namespace tape
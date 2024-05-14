#pragma once

#include "i_output_tape.h"

#include <fstream>
#include <memory>
#include <optional>
#include <string>

namespace tape {

class FileOutputTape : public IOutputTape {
  public:
    FileOutputTape(const std::string &filename);

    FileOutputTape(const FileOutputTape &) = delete;
    FileOutputTape(FileOutputTape &&) = default;
    FileOutputTape &operator=(const FileOutputTape &) = delete;
    FileOutputTape &operator=(FileOutputTape &&) = default;

    ~FileOutputTape();

    static std::shared_ptr<IOutputTape> create(const std::string &filename);

    virtual void write(std::int32_t value) override;
    virtual bool moveForward() override;
    virtual bool moveBackward() override;

  private:
    using VALUE_TYPE = std::int32_t;

    std::ofstream fStream_;
};

} // namespace tape

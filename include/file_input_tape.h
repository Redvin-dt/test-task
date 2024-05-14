#pragma once

#include "config.h"
#include "i_input_tape.h"

#include <cstddef>
#include <fstream>
#include <istream>
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

    ~FileInputTape();

    static std::shared_ptr<IInputTape> create(const std::string &filename);

    virtual std::int32_t read() override;
    virtual bool moveForward() override;
    virtual bool moveBackward() override;
    virtual void resetTape() override;
    virtual bool hasNext() override;

  private:
    using VALUE_TYPE = std::int32_t;

    bool isEof();

    std::ifstream fStream_;
    Config *config_;
};

} // namespace tape
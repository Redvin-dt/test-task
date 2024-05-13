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

    virtual bool go_next() override;
    virtual void write(std::int32_t value) override;

  private:
    void write_to_file();

    std::optional<std::int32_t> write_buf; // TODO: rename
    std::ofstream fStream_;
};

} // namespace tape

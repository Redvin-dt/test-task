#pragma once

#include "config.h"
#include "i_tape.h"

#include <fstream>
#include <memory>
#include <string>

namespace tape {

class FileTape : public ITape {
  public:
    FileTape(const std::string &filename);

    FileTape(const FileTape &) = delete;
    FileTape(FileTape &&) = default;
    FileTape &operator=(const FileTape &) = delete;
    FileTape &operator=(FileTape &&) = default;

    ~FileTape();

    static std::shared_ptr<ITape> create(const std::string &filename);

    virtual std::int32_t read() override;
    virtual void write(std::int32_t value) override;
    virtual void moveForward() override;
    virtual void moveBackward() override;
    virtual void resetTape() override;
    virtual bool hasNext() override;

  private:
    using VALUE_TYPE = std::int32_t;

    bool isEof();
    void setPosition();

    std::fstream::pos_type current_position_;
    std::fstream fStream_;
    Config *config_;
};

} // namespace tape